// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_BossMonster_Grux.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

#include "TFT_AnimInstance_Grux.h"
#include "TFT_HPBarWidget.h"

#include "TFT_GameInstance.h"
#include "TFT_UIManager.h"
#include "TFT_SoundManager.h"
#include "TFT_Player.h"

#include "UObject/ConstructorHelpers.h"
#include "AIController.h"
#include "TFT_Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "TimerManager.h"

ATFT_BossMonster_Grux::ATFT_BossMonster_Grux()
{
    _meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));

    _possessionExp = 100;
}

void ATFT_BossMonster_Grux::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = 300.0f;

    // UIMANAGER->OpenWidget(UIType::Tutorial);
    //static ConstructorHelpers::FClassFinder<UUserWidget> HpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/HP_Bar_BP.HP_Bar_BP_C'"));
    //if (HpBar.Succeeded())
    //{
    //    HpBarWidgetClass = HpBar.Class;
    //}

    // UIMANAGER->OpenWidget(UIType::Tutorial);
}

void ATFT_BossMonster_Grux::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    _statCom->SetLevelAndInit(1);

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

    if (_statCom->IsValidLowLevel())
    {
        _statCom->_deathDelegate.AddUObject(this, &ATFT_BossMonster_Grux::DeathStart);
    }
}

void ATFT_BossMonster_Grux::Tick(float DeltaTime)
{
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    /*if (Player)
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
    }*/

    /*if (_isAttacking && !bIsDashing)
    {
        SetActorLocation(LockedLocation);
        SetActorRotation(LockedRotation);
    }*/
}

void ATFT_BossMonster_Grux::SetMesh(FString path)
{
    _meshCom->SetMesh(path);
}

void ATFT_BossMonster_Grux::AttackStart()
{
    Super::AttackStart();

    SOUNDMANAGER->PlaySound("Grux_Swing", GetActorLocation());
}

void ATFT_BossMonster_Grux::AttackHit_Boss()
{
    TArray<FHitResult> hitResults;
    FCollisionQueryParams params(NAME_None, false, this);

    float attackRange = 500.0f;
    float attackRadius = 400.0f;

    bool bResult = GetWorld()->SweepMultiByChannel(
        hitResults,
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

    if (bResult)
    {
        AActor* targetActor = nullptr;

        // 필터링: "Player" 태그를 가진 액터만 처리
        for (const FHitResult& hitResult : hitResults)
        {
            AActor* hitActor = hitResult.GetActor();
            if (hitActor && hitActor->Tags.Contains(FName("Player")))
            {
                targetActor = hitActor; // "Player" 태그를 가진 액터 발견
                break; // 가장 가까운 "Player"만 타겟으로 처리
            }
        }

        if (targetActor)
        {
            // 데미지 처리
            float hpRatio = _statCom->BossHPRatio();
            float damageMultiplier = (hpRatio < 0.3f) ? 2.0f : 1.0f;

            float baseDamage = _statCom->GetAttackDamage();
            float damage = baseDamage * damageMultiplier;

            FDamageEvent damageEvent;
            targetActor->TakeDamage(damage, damageEvent, GetController(), this);
            _hitPoint = targetActor->GetActorLocation();
            drawColor = FColor::Red;

            // 상태 효과 적용
            /*ATFT_Creature* targetCreature = Cast<ATFT_Creature>(targetActor);
            if (targetCreature != nullptr)
            {
                switch (_curAttackIndex)
                {
                case 1:
                    targetCreature->SetState(StateType::Airborne);
                    break;
                case 2:
                    targetCreature->SetState(StateType::Stun);
                    break;
                case 3:
                    targetCreature->SetState(StateType::Slow);
                    break;
                default:
                    break;
                }
            }*/
        }
    }

    DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_BossMonster_Grux::Attack_AI()
{
    Super::Attack_AI();

    if (!_isAttacking && _animInstance_Grux != nullptr)
    {
        if (!_animInstance_Grux->Montage_IsPlaying(_animInstance_Grux->_attackMontage) &&
            !_animInstance_Grux->Montage_IsPlaying(_animInstance_Grux->_skillMontage))
        {
            int32 randValue = FMath::RandRange(0, 10);

            if (randValue > 1)
            {
                _animInstance_Grux->bIsDashing = false;

                LockedLocation = GetActorLocation();
                LockedRotation = GetActorRotation();

                _animInstance_Grux->PlayAttackMontage();

                _isAttacking = true;

                _curAttackIndex %= 2;
                _curAttackIndex++;
                _animInstance_Grux->JumpToSection(_curAttackIndex);
            }
            else
            {
                _animInstance_Grux->bIsDashing = true;
                bIsDashing = true;

                _animInstance_Grux->PlaySkillMontage();

                GetCharacterMovement()->MaxWalkSpeed = _dashSpeed;
                GetCharacterMovement()->RotationRate = FRotator(0.0f, 900.0f, 0.0f);
            }

            // _animInstance_Grux->OnMontageEnded.AddDynamic(this, &ATFT_BossMonster_Grux::ResetMovementLock);
        }
    }
}

void ATFT_BossMonster_Grux::AttackEnd()
{
    bIsDashing = false;
    _animInstance_Grux->bIsDashing = false;
    GetCharacterMovement()->MaxWalkSpeed = _walkSpeed;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

float ATFT_BossMonster_Grux::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    return ActualDamage;
}

void ATFT_BossMonster_Grux::DeathStart()
{
    Super::DeathStart();

    // _animInstance_Grux->StopAllMontages(0.0f);

    _animInstance_Grux->PlayDeathMontage();

    GetWorldTimerManager().SetTimer(_deathTimerHandle, this, &ATFT_BossMonster_Grux::BossDisable, 2.0f, false);

    // UIMANAGER->CloseWidget(UIType::Tutorial);
}

void ATFT_BossMonster_Grux::BossDisable()
{
    Super::DropItem(MonsterType::Normal);

    PrimaryActorTick.bCanEverTick = false;

    if (HpBarWidgetInstance)
    {
        HpBarWidgetInstance->RemoveFromParent();
        HpBarWidgetInstance = nullptr;
    }

    DeActive();
}

void ATFT_BossMonster_Grux::StateCheck()
{
    Super::StateCheck();

    if (_statCom->IsDead()) return;

    auto curStates = _stateCom->GetCurStates();

    if (curStates.IsEmpty()) return;

    auto controller = Cast<ATFT_Boss_AIController>(GetController());
    
    if (controller)
    {
        controller->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsOnState")), true);
    }

    /*for (auto state : curStates)
    {
        switch (state)
        {
        case StateType::Airborne:

            bIsOnState = true;
            _animInstance_Grux->PlayAirborneMontage();
            _canMove = false;
            _stateCom->InitState();
            GetWorldTimerManager().SetTimer(_stateTimerHandle, this, &ATFT_BossMonster_Grux::EndState, 2.0f, false);
            break;

        case StateType::Stun:
            bIsOnState = true;
            _animInstance_Grux->PlayStunMontage();
            _canMove = false;
            _stateCom->InitState();
            GetWorldTimerManager().SetTimer(_stateTimerHandle, this, &ATFT_BossMonster_Grux::EndState, 2.0f, false);
            break;

        default:
            break;
        }
    }*/
}

void ATFT_BossMonster_Grux::EndState()
{
    Super::EndState();

    auto controller = Cast<ATFT_Boss_AIController>(GetController());

    if (controller)
    {
        controller->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsOnState")), false);
    }

    bIsOnState = false;

    GetWorldTimerManager().ClearTimer(_stateTimerHandle);
}

void ATFT_BossMonster_Grux::SetAnimInstanceBind()
{
    Super::SetAnimInstanceBind();
    
    if (!bAnimBind)
    {
        _animInstance_Grux = Cast<UTFT_AnimInstance_Grux>(GetMesh()->GetAnimInstance());

        if (_animInstance_Grux->IsValidLowLevel())
        {
            _animInstance_Grux->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
            _animInstance_Grux->_attackStartDelegate.AddUObject(this, &ATFT_BossMonster_Grux::AttackStart);
            _animInstance_Grux->_attackHitDelegate.AddUObject(this, &ATFT_BossMonster_Grux::AttackHit_Boss);
            _animInstance_Grux->_attackEndDelegate.AddUObject(this, &ATFT_BossMonster_Grux::AttackEnd);
            // _animInstance_Grux->_deathStartDelegate.AddUObject(this, &ATFT_BossMonster_Grux::DeathStart);
            _animInstance_Grux->_deathEndDelegate.AddUObject(this, &ATFT_BossMonster_Grux::BossDisable);
            _animInstance_Grux->_stateMontageEndDelegate.AddUObject(this, &ATFT_BossMonster_Grux::EndState);

            bAnimBind = true;
        }
    }
}

void ATFT_BossMonster_Grux::Active()
{
    Super::Active();
  
}

void ATFT_BossMonster_Grux::DeActive()
{
    Super::DeActive();

}
