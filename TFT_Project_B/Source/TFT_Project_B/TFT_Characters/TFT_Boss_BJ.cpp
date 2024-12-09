// Fill out your copyright notice in the Description page of Project Settings.

#include "TFT_Characters/TFT_Boss_BJ.h"
#include "TFT_Player.h"
#include "TFT_AnimInstances/TFT_AnimInstance_BJ.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "TFT_SoundManager.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "TimerManager.h"
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

}

void ATFT_Boss_BJ::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    _statCom->SetLevelAndInit(1);

    _animInstance_BJ = Cast<UTFT_AnimInstance_BJ>(GetMesh()->GetAnimInstance());
    if (_animInstance_BJ->IsValidLowLevel())
    {
        _animInstance_BJ->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
        _animInstance_BJ->_attackStartDelegate.AddUObject(this, &ATFT_Boss_BJ::AttackStart);
        _animInstance_BJ->_attackHitDelegate.AddUObject(this, &ATFT_Boss_BJ::AttackHit_Boss);
        _animInstance_BJ->_deathStartDelegate.AddUObject(this, &ATFT_Boss_BJ::DeathStart);
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

void ATFT_Boss_BJ::AttackStart()
{
    Super::AttackStart();

    SOUNDMANAGER->PlaySound("Boss_Attack", GetActorLocation());
}

void ATFT_Boss_BJ::FootStep()
{
    	Super::FootStep();

	auto player = GetWorld()->GetFirstPlayerController()->GetOwner();

	FVector start = GetActorLocation();
	FRotator rotator = GetActorRotation();
	FVector lineDirAndDist = FVector(1.0f, 1.0f, -100.0f);
	FVector end = start * lineDirAndDist;
	FHitResult hitResult;

	FCollisionQueryParams qParams;
	qParams.AddIgnoredActor(this);
	qParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel
	(
		hitResult,
		start,
		end,
		ECollisionChannel::ECC_Visibility,
		qParams
	);

	if (hitResult.PhysMaterial != nullptr)
	{

		FString hitName = hitResult.PhysMaterial->GetName();

		// UE_LOG(LogTemp, Log, TEXT("%s"), *hitName);
	}

	if (hitResult.PhysMaterial != nullptr)
	{
		switch (hitResult.PhysMaterial->SurfaceType)
		{
		case SurfaceType1:
			SOUNDMANAGER->FadeOutSound("Boss_FootStep", 0.2f);
			SOUNDMANAGER->PlaySound("Boss_FootStep", end);
			break;
		default:
			break;
		}
	}
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

            if (randomValue < 10)
            {
                if (!_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_skillMontage) &&
                    !_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_slashMontage))
                {
                    _animInstance_BJ->PlaySkillMontage();
                    _isAttacking = true;
                    //SOUNDMANAGER->PlaySound("", GetActorLocation());
                }
            }
            else if (randomValue < 20)
            {
                GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &ATFT_Boss_BJ::MoveMessageForward, 1.0f, false);
                _animInstance_BJ->PlaySlashMontage();
                _isAttacking = true;
                SOUNDMANAGER->PlaySound("Boss_Dash", GetActorLocation());
            }
            else if (randomValue < 30)
            {
                if (!_animInstance_BJ->Montage_IsPlaying(_animInstance_BJ->_skillMontage))
                {
                    ActivateSkill();
                    //SOUNDMANAGER->PlaySound("Boss_Elec", GetActorLocation());
                }
            }
            else
            {
                _animInstance_BJ->PlayAttackMontage();
                _isAttacking = true;
                SOUNDMANAGER->PlaySound("Boss_Attack", GetActorLocation());
            }

            _curAttackIndex %= 4;
            _curAttackIndex++;
            _animInstance_BJ->JumpToSection(_curAttackIndex);
        }
    }
}

void ATFT_Boss_BJ::MoveMessageForward()
{
    FVector currentLocation = GetActorLocation();
    FVector newLocation = currentLocation + GetActorForwardVector() * 500.0f;
    SetActorLocation(newLocation);
}

void ATFT_Boss_BJ::AttackEnd()
{
}

//void ATFT_Boss_BJ::DropItem()
//{
//}

float ATFT_Boss_BJ::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);


    return ActualDamage;
}

void ATFT_Boss_BJ::DeathStart()
{
    Super::DeathStart();

    _animInstance_BJ->_deathStartDelegate.RemoveAll(this);
}

void ATFT_Boss_BJ::BossDisable()
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

void ATFT_Boss_BJ::ActivateSkillRandom()
{
    if (!_isAttacking && _animInstance_BJ != nullptr)
    {
        FVector Origin = GetActorLocation();
        FVector RandomOffset = FVector(FMath::RandRange(-500.f, 500.f), FMath::RandRange(-500.f, 500.f), 0.f);
        TargetLocation = Origin + RandomOffset;

        if (FVector::Dist(TargetLocation, Origin) < 500.f)
        {
            SpawnNiagaraEffectAtLocation(TargetLocation);

            _animInstance_BJ->PlaySkillMontage();
            _isAttacking = true;

            GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &ATFT_Boss_BJ::TriggerSkillEffect, 2.f, false);
        }
    }
}

void ATFT_Boss_BJ::ActivateSkill()
{
    if (!_isAttacking && _animInstance_BJ != nullptr)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            APawn* PlayerPawn = PlayerController->GetPawn();
            if (PlayerPawn)
            {
                ATFT_Player* Player = Cast<ATFT_Player>(PlayerPawn);
                if (Player)
                {
                    TargetLocation = Player->GetActorLocation();
                    SpawnNiagaraEffectAtLocation(TargetLocation);

                    _animInstance_BJ->PlaySkillMontage();
                    _isAttacking = true;

                    GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &ATFT_Boss_BJ::TriggerSkillEffect, 2.f, false);
                }
            }
        }
    }
}

void ATFT_Boss_BJ::SpawnNiagaraEffectAtLocation(FVector Location)
{
    UNiagaraSystem* NiagaraEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/BeamPack/Niagara/HitPoint/NS_Tornado_Thunder1.NS_Tornado_Thunder1'"));
    if (NiagaraEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, Location, FRotator::ZeroRotator, FVector(1.0f), true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Niagara System for ThunderEffect."));
    }
}

void ATFT_Boss_BJ::TriggerSkillEffect()
{
    _isAttacking = false;

    if (SOUNDMANAGER)
    {
        SOUNDMANAGER->PlaySound("Boss_Elec", GetActorLocation());
    }

    UNiagaraSystem* NiagaraEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Electric_VFX/Niagara/Electric/Boss_Elec.Boss_Elec'"));
    if (NiagaraEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, TargetLocation);

        TArray<AActor*> ActorsToDamage;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATFT_Player::StaticClass(), ActorsToDamage);

        for (AActor* Actor : ActorsToDamage)
        {
            if (Actor && FVector::Dist(Actor->GetActorLocation(), TargetLocation) < 300.f)
            {
                UGameplayStatics::ApplyDamage(Actor, 20.f, GetController(), this, UDamageType::StaticClass());
            }
        }
    }
}