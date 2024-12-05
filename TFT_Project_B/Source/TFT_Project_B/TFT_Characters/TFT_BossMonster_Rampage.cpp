// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_BossMonster_Rampage.h"
#include "TFT_AnimInstances/TFT_AnimInstance_Rampage.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "TFT_Characters/TFT_Player.h"
#include "../TFT_Components/TFT_StatComponent.h"

#include "Engine/DamageEvents.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h" 
#include "TFT_NPC2.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ATFT_BossMonster_Rampage::ATFT_BossMonster_Rampage()
{
    _meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));

	SetMesh("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");

    static ConstructorHelpers::FClassFinder<UUserWidget> HpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/HP_Bar_BP.HP_Bar_BP_C'"));
    if (HpBar.Succeeded())
    {
        HpBarWidgetClass = HpBar.Class;
    }


    armcapsule_R = CreateDefaultSubobject<UCapsuleComponent>(TEXT("armcapsule_R"));
    armcapsule_R->SetupAttachment(GetMesh(), TEXT("arm_R"));

    armcapsule_L = CreateDefaultSubobject<UCapsuleComponent>(TEXT("armcapsule_L"));
    armcapsule_L->SetupAttachment(GetMesh(), TEXT("arm_L"));

    armcapsule_R->SetCapsuleRadius(10.f);
    armcapsule_R->SetCapsuleHalfHeight(30.f);

    armcapsule_L->SetCapsuleRadius(10.f);
    armcapsule_L->SetCapsuleHalfHeight(30.f);

}

void ATFT_BossMonster_Rampage::BeginPlay()
{
    Super::BeginPlay();

    _statCom->SetLevelAndInit(1);
    PlayerController = GetWorld()->GetFirstPlayerController();
}

void ATFT_BossMonster_Rampage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	_animInstance_Boss = Cast<UTFT_AnimInstance_Rampage>(GetMesh()->GetAnimInstance());
    if (_animInstance_Boss->IsValidLowLevel())
    {
        _animInstance_Boss->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
        _animInstance_Boss->_attackStartDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::AttackStart);
        _animInstance_Boss->_attackHitDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::AttackHit_Boss);
        _animInstance_Boss->_deathStartDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::DeathStart);
        _animInstance_Boss->_deathEndDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::BossDisable);
        _animInstance_Boss->OnExplosionHitDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::ExplosionHit);
        _animInstance_Boss->OnChainExplosionHitDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::ChainExplosionHit);
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
            HpBar->SetProfileImage(ProfileType::BOSS1);
            HpBar->SetHpText(_statCom->GetMaxHp());
            _statCom->_BosshpChangedDelegate.AddUObject(HpBar, &UTFT_HPBarWidget::SetHpBarValue);
            _statCom->_CurHpText.AddUObject(HpBar, &UTFT_HPBarWidget::CurHpText);
        }
    }

}

void ATFT_BossMonster_Rampage::Tick(float DeltaTime)
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

    if (_isAttacking)
    {
        SetActorLocation(LockedLocation);
    }
}

void ATFT_BossMonster_Rampage::SetMesh(FString path)
{
	_meshCom->SetMesh(path);
}

void ATFT_BossMonster_Rampage::AttackHit_Boss()
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
        ECollisionChannel::ECC_GameTraceChannel6,
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

void ATFT_BossMonster_Rampage::Attack_AI()
{
    Super::Attack_AI();

    if (!_isAttacking && _animInstance_Boss != nullptr)
    {
        if (!_animInstance_Boss->Montage_IsPlaying(_animInstance_Boss->_myAnimMontage) &&
            !_animInstance_Boss->Montage_IsPlaying(_animInstance_Boss->_skillMontage) &&
            !_animInstance_Boss->Montage_IsPlaying(_animInstance_Boss->_JumpskillMontage))
        {
            LockedLocation = GetActorLocation();

            if (bCanUseJumpSkill)
            {
                _animInstance_Boss->PlayJumpSkillMontage();
                bCanUseJumpSkill = false;
                GetWorld()->GetTimerManager().SetTimer(JumpSkillCooldownTimerHandle, this, &ATFT_BossMonster_Rampage::ResetJumpSkillCooldown, JumpSkillCooldown, false);
            }
            else if (bCanUseSkill)
            {
                _animInstance_Boss->PlaySkillMontage();
                bCanUseSkill = false;
                GetWorld()->GetTimerManager().SetTimer(SkillCooldownTimerHandle, this, &ATFT_BossMonster_Rampage::ResetSkillCooldown, SkillCooldown, false);
            }
            else
            {
                _animInstance_Boss->PlayAttackMontage();
            }

            _isAttacking = true;

            _curAttackIndex %= 3;
            _curAttackIndex++;
            _animInstance_Boss->JumpToSection(_curAttackIndex);

            _animInstance_Boss->OnMontageEnded.AddDynamic(this, &ATFT_BossMonster_Rampage::ResetMovementLock);
        }
    }
   
}

void ATFT_BossMonster_Rampage::ResetSkillCooldown()
{
    bCanUseSkill = true;
}

void ATFT_BossMonster_Rampage::ResetJumpSkillCooldown()
{
    bCanUseJumpSkill = true;
}

void ATFT_BossMonster_Rampage::AttackEnd()
{
}

void ATFT_BossMonster_Rampage::DropItem()
{
}

float ATFT_BossMonster_Rampage::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);


    return ActualDamage;
}

void ATFT_BossMonster_Rampage::DeathStart()
{
    Super::DeathStart();
    if (GetWorld())
    {
        for (TActorIterator<ATFT_NPC2> NPC2Itr(GetWorld()); NPC2Itr; ++NPC2Itr)
        {
            ATFT_NPC2* NPC2 = *NPC2Itr;
            if (NPC2) 
            {
                NPC2->ShowNPC();
                UE_LOG(LogTemp, Log, TEXT("Boss died. NPC2 is now visible."));
                break; 
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr! Cannot find NPC2."));
    }

    _animInstance_Boss->_deathStartDelegate.RemoveAll(this);
}

void ATFT_BossMonster_Rampage::ResetMovementLock(UAnimMontage* Montage, bool bInterrupted)
{
    _isAttacking = false;

    _animInstance_Boss->OnMontageEnded.RemoveDynamic(this, &ATFT_BossMonster_Rampage::ResetMovementLock);
}



void ATFT_BossMonster_Rampage::BossDisable()
{
    this->SetActorHiddenInGame(true);

    _animInstance_Boss->_deathEndDelegate.RemoveAll(this);
    //_animInstance_Boss->_attackStartDelegate.RemoveAll(this);
    //_animInstance_Boss->_attackHitDelegate.RemoveAll(this);

    PrimaryActorTick.bCanEverTick = false;
    auto controller = GetController();
    if (controller != nullptr) GetController()->UnPossess();

    if (HpBarWidgetInstance)
    {
        HpBarWidgetInstance->RemoveFromParent();
        HpBarWidgetInstance = nullptr;
    }
}

void ATFT_BossMonster_Rampage::UpdateBlackboardTarget()
{
}

void ATFT_BossMonster_Rampage::ExplosionHit()
{
    FHitResult hitResult;
    FCollisionQueryParams params(NAME_None, false, this);

    float explosionRange = 500.0f; // Explosion 범위
    float explosionRadius = 150.0f; // Explosion 반경

    bool bResult = GetWorld()->SweepSingleByChannel(
        hitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * explosionRange,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel6,
        FCollisionShape::MakeSphere(explosionRadius),
        params
    );

    FVector vec = GetActorForwardVector() * explosionRange;
    FVector center = GetActorLocation() + vec * 0.5f;
    FColor drawColor = FColor::Green;

    if (bResult && hitResult.GetActor()->IsValidLowLevel())
    {
        drawColor = FColor::Red;
        FDamageEvent damageEvent;

        float explosionDamage = 30.0f; // Explosion 데미지
        hitResult.GetActor()->TakeDamage(explosionDamage, damageEvent, GetController(), this);
        _hitPoint = hitResult.ImpactPoint;
    }

    DrawDebugSphere(GetWorld(), center, explosionRadius, 20, drawColor, false, 0.1f);

}

void ATFT_BossMonster_Rampage::ChainExplosionHit()
{
    FHitResult hitResult;
    FCollisionQueryParams params(NAME_None, false, this);

    float chainRange = 600.0f; // Chain Explosion 범위
    float chainRadius = 150.0f; // Chain Explosion 반경

    // 첫 번째 구체 처리
    bool bResult = GetWorld()->SweepSingleByChannel(
        hitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * chainRange,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel6,
        FCollisionShape::MakeSphere(chainRadius),
        params
    );

    FVector vec = GetActorForwardVector() * chainRange;
    FVector center = GetActorLocation() + vec * 0.5f;
    FColor drawColor = FColor::Green;

    if (bResult && hitResult.GetActor()->IsValidLowLevel())
    {
        drawColor = FColor::Red;
        FDamageEvent damageEvent;

        float chainExplosionDamage = 20.0f; // 첫 번째 구체 데미지
        hitResult.GetActor()->TakeDamage(chainExplosionDamage, damageEvent, GetController(), this);
        _hitPoint = hitResult.ImpactPoint;
    }

    DrawDebugSphere(GetWorld(), center, chainRadius, 20, drawColor, false, 0.1f);

    // 추가 구체 처리 (첫 번째 구체의 앞부분)
    FHitResult secondHitResult;
    FVector extendedStartLocation = GetActorLocation() + GetActorForwardVector() * chainRange;
    FVector extendedEndLocation = extendedStartLocation + GetActorForwardVector() * (chainRange * 0.5f);
    FVector secondCenter = (extendedStartLocation + extendedEndLocation) * 0.5f;

    bool bSecondHit = GetWorld()->SweepSingleByChannel(
        secondHitResult,
        extendedStartLocation,
        extendedEndLocation,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel6,
        FCollisionShape::MakeSphere(chainRadius),
        params
    );

    FColor secondDrawColor = FColor::Blue;

    if (bSecondHit && secondHitResult.GetActor()->IsValidLowLevel())
    {
        secondDrawColor = FColor::Red;
        FDamageEvent secondDamageEvent;

        float secondExplosionDamage = 20.0f; // 두 번째 구체 데미지
        secondHitResult.GetActor()->TakeDamage(secondExplosionDamage, secondDamageEvent, GetController(), this);
    }

    DrawDebugSphere(GetWorld(), secondCenter, chainRadius, 20, secondDrawColor, false, 0.1f);

}
