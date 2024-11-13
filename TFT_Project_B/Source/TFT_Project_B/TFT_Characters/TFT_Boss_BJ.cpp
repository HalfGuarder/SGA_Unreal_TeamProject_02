// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_Boss_BJ.h"
#include "TFT_AnimInstances/TFT_AnimInstance_BJ.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "Engine/DamageEvents.h"
#include "TFT_GameInstance.h"
#include "TFT_EffectManager.h"

ATFT_Boss_BJ::ATFT_Boss_BJ()
{
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));

    ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/SciFi_Soldiers/Mesh/Male/SK_SciFi_Soldier_Male_Skin4.SK_SciFi_Soldier_Male_Skin4'"));
    if (SkeletalMeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> HpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/HP_Bar_BP.HP_Bar_BP_C'"));
    if (HpBar.Succeeded())
    {
        HpBarWidgetClass = HpBar.Class;
    }
    

}

void ATFT_Boss_BJ::BeginPlay()
{
    Super::BeginPlay();

    _statCom->SetLevelAndInit(1);
}

void ATFT_Boss_BJ::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    _animInstance_BJ = Cast<UTFT_AnimInstance_BJ>(GetMesh()->GetAnimInstance());
    if (_animInstance_BJ->IsValidLowLevel())
    {
        _animInstance_BJ->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
        _animInstance_BJ->_attackStartDelegate.AddUObject(this, &ATFT_Boss_BJ::AttackStart);
        _animInstance_BJ->_attackHitDelegate.AddUObject(this, &ATFT_Boss_BJ::AttackHit_Boss);
        _animInstance_BJ->_deathStartDelegate.AddUObject(this, &ATFT_Boss_BJ::DeathStart);
        _animInstance_BJ->_bossDeathEndDelegate.AddUObject(this, &ATFT_Boss_BJ::Boss_DeathEnd);
        _animInstance_BJ->_deathEndDelegate.AddUObject(this, &ATFT_Boss_BJ::BossDisable);
    }

    if (HpBarWidgetClass)
    {
        HpBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HpBarWidgetClass);
        if (HpBarWidgetInstance)
        {
            HpBarWidgetInstance->AddToViewport();
        }
    }

    if (HpBarWidgetInstance)
    {

        UTFT_HPBarWidget* HpBar = Cast<UTFT_HPBarWidget>(HpBarWidgetInstance);
        if (HpBar)
        {
            HpBar->SetProfileImage(ProfileType::BOSS2);
            HpBar->SetHpText(_statCom->GetMaxHp());
            _statCom->_BosshpChangedDelegate.AddUObject(HpBar, &UTFT_HPBarWidget::SetHpBarValue);
            _statCom->_CurHpText.AddUObject(HpBar, &UTFT_HPBarWidget::CurHpText);
        }
    }
}

void ATFT_Boss_BJ::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (Player)
    {
        float Distance = FVector::Dist(Player->GetActorLocation(), GetActorLocation());

        if (HpBarWidgetInstance)
        {
            if (Distance <= 1000.0f)
            {
                HpBarWidgetInstance->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                HpBarWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }

}

void ATFT_Boss_BJ::SetMesh(FString path)
{
    _meshCom->SetMesh(path);
}

void ATFT_Boss_BJ::AttackHit_Boss()
{
    FHitResult hitResult;
    FCollisionQueryParams params(NAME_None, false, this);

    float attackRange = 500.0f;
    float attackRadius = 100.0f;

    bool bResult = GetWorld()->SweepSingleByChannel(
        hitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * attackRange,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel10,
        FCollisionShape::MakeSphere(attackRadius),
        params
    );

    FVector vec = GetActorForwardVector() * attackRange;
    FVector center = GetActorLocation() + vec * 0.5f;

    FColor drawColor = FColor::Green;

    if (bResult && hitResult.GetActor()->IsValidLowLevel())
    {
        float hpRatio = _statCom->BossHPRatio();
        float damageMultiplier = (hpRatio < 0.3f) ? 2.0f : 1.0f;

        float baseDamage = _statCom->GetAttackDamage();
        float damage = baseDamage * damageMultiplier;

        FDamageEvent damageEvent;
        hitResult.GetActor()->TakeDamage(damage, damageEvent, GetController(), this);
        _hitPoint = hitResult.ImpactPoint;
        drawColor = FColor::Red;

        EFFECTMANAGER->Play("N_Monster_Boss_Attack_Hit", 1, _hitPoint);


        ATFT_Creature* target = Cast<ATFT_Creature>(hitResult.GetActor());
        if (target != nullptr)
        {
            switch (_curAttackIndex)
            {
            case 1:
                target->SetState(StateType::Airborne);
                break;
            case 2:
                target->SetState(StateType::Stun);
                break;
            case 3:
                target->SetState(StateType::Slow);
                break;
            default:
                break;
            }
        }
    }

    DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_Boss_BJ::Attack_AI()
{
    Super::Attack_AI();

    if (!_isAttacking && _animInstance_BJ != nullptr)
    {
        if (!_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_myAnimMontage) &&
            !_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_skillMontage) &&
            !_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_slashMontage))
        {
            int32 randomValue = FMath::RandRange(0, 100);

            if (randomValue < 15)
            {
                _animInstance_BJ->PlaySkillMontage();
            }
            else if (randomValue < 30)
            {
                _animInstance_BJ->PlaySlashMontage();
            }
            else
            {
                _animInstance_BJ->PlayAttackMontage();
            }

            _isAttacking = true;

            _curAttackIndex %= 4;
            _curAttackIndex++;
            _animInstance_BJ->JumpToSection(_curAttackIndex);
        }
    }
} 

void ATFT_Boss_BJ::AttackEnd()
{
}

void ATFT_Boss_BJ::DropItem()
{
}

float ATFT_Boss_BJ::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);


    return ActualDamage;
}

void ATFT_Boss_BJ::DeathStart()
{
    this->SetActorHiddenInGame(true);

    _animInstance_BJ->_bossDeathEndDelegate.RemoveAll(this);
}

void ATFT_Boss_BJ::Boss_DeathEnd()
{
}

void ATFT_Boss_BJ::BossDisable()
{
}