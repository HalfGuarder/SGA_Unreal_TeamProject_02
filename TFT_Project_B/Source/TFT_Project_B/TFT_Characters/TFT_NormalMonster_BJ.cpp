// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_NormalMonster_BJ.h"

#include "TFT_Player.h"
#include "TFT_AnimInstances/TFT_AnimInstance_NormalBJ.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "TFT_SoundManager.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "TFT_GameInstance.h"
#include "TFT_EffectManager.h"

ATFT_NormalMonster_BJ::ATFT_NormalMonster_BJ()
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

void ATFT_NormalMonster_BJ::BeginPlay()
{
    Super::BeginPlay();

    _statCom->SetLevelAndInit(2);

    GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}

void ATFT_NormalMonster_BJ::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    _animInstance_BJ = Cast<UTFT_AnimInstance_NormalBJ>(GetMesh()->GetAnimInstance());
    if (_animInstance_BJ->IsValidLowLevel())
    {
        _animInstance_BJ->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
        _animInstance_BJ->_attackStartDelegate.AddUObject(this, &ATFT_NormalMonster_BJ::AttackStart);
        _animInstance_BJ->_attackHitDelegate.AddUObject(this, &ATFT_NormalMonster_BJ::AttackHit_Boss);
        _animInstance_BJ->_deathStartDelegate.AddUObject(this, &ATFT_NormalMonster_BJ::DeathStart);
        _animInstance_BJ->_deathEndDelegate.AddUObject(this, &ATFT_NormalMonster_BJ::BossDisable);
    }

    //if (HpBarWidgetClass)
    //{
    //    HpBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HpBarWidgetClass);
    //    if (HpBarWidgetInstance)
    //    {
    //        HpBarWidgetInstance->AddToViewport();
    //    }
    //}

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

void ATFT_NormalMonster_BJ::Tick(float DeltaTime)
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

void ATFT_NormalMonster_BJ::SetMesh(FString path)
{
    _meshCom->SetMesh(path);
}

void ATFT_NormalMonster_BJ::AttackStart()
{
    Super::AttackStart();

    SOUNDMANAGER->PlaySound("Boss_Attack", GetActorLocation());
}

void ATFT_NormalMonster_BJ::AttackHit_Boss()
{
    FHitResult hitResult;
    FCollisionQueryParams params(NAME_None, false, this);

    float attackRange = 500.0f;
    float attackRadius = 400.0f;

    bool bResult = GetWorld()->SweepSingleByChannel(
        hitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * attackRange,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel6,
        FCollisionShape::MakeSphere(attackRadius),
        params
    );

    FVector vec = GetActorForwardVector() * attackRange;
    FVector center = GetActorLocation() + vec * 0.5f;

    FColor drawColor = FColor::Green;

    if (bResult && hitResult.GetActor()->IsValidLowLevel())
    {
        AActor* hitActor = hitResult.GetActor();

        // "Player" 태그를 가진 액터만 공격 처리
        if (hitActor->Tags.Contains(FName("Player")))
        {
            float hpRatio = _statCom->BossHPRatio();
            float damageMultiplier = (hpRatio < 0.3f) ? 2.0f : 1.0f;

            float baseDamage = _statCom->GetAttackDamage();
            float damage = baseDamage * damageMultiplier;

            FDamageEvent damageEvent;
            hitActor->TakeDamage(damage, damageEvent, GetController(), this);
            _hitPoint = hitResult.ImpactPoint;
            drawColor = FColor::Red;

            EFFECTMANAGER->Play("N_Monster_Boss_Attack_Hit", 1, _hitPoint);

            ATFT_Creature* target = Cast<ATFT_Creature>(hitActor);
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
        // "Monster" 태그를 가진 액터는 데미지를 무시
        else if (hitActor->Tags.Contains(FName("Monster")))
        {
            UE_LOG(LogTemp, Warning, TEXT("Ignored attack damage on another monster: %s"), *hitActor->GetName());
        }
    }

    DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_NormalMonster_BJ::Attack_AI()
{
    Super::Attack_AI();

    if (!_isAttacking && _animInstance_BJ != nullptr)
    {
        // 스킬 애니메이션이 재생 중이 아닌 경우에만 실행
        if (!_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_myAnimMontage))
        {
            // 일반 공격 애니메이션 실행
            _animInstance_BJ->PlayAttackMontage();
            _isAttacking = true;

            // 공격 사운드 재생
            SOUNDMANAGER->PlaySound("Boss_Attack", GetActorLocation());

            // 공격 인덱스 업데이트
            _curAttackIndex %= 4;
            _curAttackIndex++;
            _animInstance_BJ->JumpToSection(_curAttackIndex);
        }
    }
}

void ATFT_NormalMonster_BJ::AttackEnd()
{
}

float ATFT_NormalMonster_BJ::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);


    return ActualDamage;
}

void ATFT_NormalMonster_BJ::DeathStart()
{
    Super::DeathStart();

    _animInstance_BJ->_deathStartDelegate.RemoveAll(this);
}

void ATFT_NormalMonster_BJ::BossDisable()
{
    Super::DropItem(MonsterType::BOSS);

    this->SetActorHiddenInGame(true);

    _animInstance_BJ->_deathEndDelegate.RemoveAll(this);
    //_animInstance_BJ->_attackStartDelegate.RemoveAll(this);
    //_animInstance_BJ->_attackHitDelegate.RemoveAll(this);

    PrimaryActorTick.bCanEverTick = false;
    auto controller = GetController();
    if (controller != nullptr) GetController()->UnPossess();

    if (HpBarWidgetInstance)
    {
        HpBarWidgetInstance->RemoveFromParent();
        HpBarWidgetInstance = nullptr;
    }
}
