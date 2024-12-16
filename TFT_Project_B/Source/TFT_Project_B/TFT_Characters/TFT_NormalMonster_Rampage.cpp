// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_NormalMonster_Rampage.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "TFT_Characters/TFT_Player.h"
#include "../TFT_Components/TFT_StatComponent.h"
#include "TFT_AnimInstances/TFT_AnimInstance_NormalRampage.h"
#include "TFT_Characters/TFT_Creature.h"
#include "TFT_Characters/TFT_Monster.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "Engine/DamageEvents.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h" 

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ATFT_NormalMonster_Rampage::ATFT_NormalMonster_Rampage()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> HpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/HP_Bar_BP.HP_Bar_BP_C'"));
    if (HpBar.Succeeded())
    {
        HpBarWidgetClass = HpBar.Class;
    }

    _possessionExp = 100;
}

void ATFT_NormalMonster_Rampage::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = GetWorld()->GetFirstPlayerController();

    if (GetMesh())
    {
        GetMesh()->SetAnimInstanceClass(UTFT_AnimInstance_NormalRampage::StaticClass());
    }

    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ATFT_NormalMonster_Rampage::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (_statCom->IsValidLowLevel())
    {
        _statCom->SetLevelAndInit(1);
        _statCom->_deathDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::DeathStart);
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
            HpBar->SetProfileImage(ProfileType::BOSS1);
            HpBar->SetHpText(_statCom->GetMaxHp());
            _statCom->_BosshpChangedDelegate.AddUObject(HpBar, &UTFT_HPBarWidget::SetHpBarValue);
            _statCom->_CurHpText.AddUObject(HpBar, &UTFT_HPBarWidget::CurHpText);
        }
    }
}

void ATFT_NormalMonster_Rampage::Tick(float DeltaTime)
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

void ATFT_NormalMonster_Rampage::SetMesh(FString path)
{
    _meshCom->SetMesh(path);
}

void ATFT_NormalMonster_Rampage::AttackHit_Boss()
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
            ATFT_Creature* targetCreature = Cast<ATFT_Creature>(targetActor);
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
            }
        }
    }

    DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_NormalMonster_Rampage::Attack_AI()
{
    if (!_isAttacking && _animInstance_Rampage != nullptr)
    {
        // �ٸ� ��Ÿ�ְ� ���� ������ ���� ���� ����
        if (!_animInstance_Rampage->Montage_IsPlaying(_animInstance_Rampage->_attackMontage))
        {
            // ���� ��ġ�� ��� ó��
            LockedLocation = GetActorLocation();

            // �Ϲ� ���� ��Ÿ�� ���
            _animInstance_Rampage->PlayAttackMontage();

            _isAttacking = true;

            // ���� ���� ��ȯ ó��
            _curAttackIndex %= 3;
            _curAttackIndex++;
            _animInstance_Rampage->JumpToSection(_curAttackIndex);

            // ��Ÿ�� ���� �� ������ ó���ϴ� ��������Ʈ �߰�
            //_animInstance_Boss->OnMontageEnded.AddDynamic(this, &ATFT_BossMonster_Rampage::ResetMovementLock);
        }
    }
}

void ATFT_NormalMonster_Rampage::AttackEnd()
{
}

void ATFT_NormalMonster_Rampage::DropItem()
{
}

float ATFT_NormalMonster_Rampage::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);


    return ActualDamage;
}

void ATFT_NormalMonster_Rampage::DeathStart()
{
    Super::DeathStart();

    // _animInstance_Boss->_deathStartDelegate.RemoveAll(this);

    if (_animInstance_Rampage)
    {
        _animInstance_Rampage->PlayDeathMontage();
    }
    else
    {
        return;
    }

    
    GetWorldTimerManager().SetTimer(_deathTimerHandle, this, &ATFT_NormalMonster_Rampage::BossDisable, 2.0f, false);
}

void ATFT_NormalMonster_Rampage::ResetMovementLock(UAnimMontage* Montage, bool bInterrupted)
{
    _isAttacking = false;

    _animInstance_Rampage->OnMontageEnded.RemoveDynamic(this, &ATFT_NormalMonster_Rampage::ResetMovementLock);
}

void ATFT_NormalMonster_Rampage::BossDisable()
{
    Super::DropItem(MonsterType::Normal);

    // this->SetActorHiddenInGame(true);

    // _animInstance_Rampage->_deathEndDelegate.RemoveAll(this);
    //_animInstance_Boss->_attackStartDelegate.RemoveAll(this);
    //_animInstance_Boss->_attackHitDelegate.RemoveAll(this);

    PrimaryActorTick.bCanEverTick = false;
    //auto controller = GetController();
    //if (controller != nullptr) GetController()->UnPossess();

    if (HpBarWidgetInstance)
    {
        HpBarWidgetInstance->RemoveFromParent();
        HpBarWidgetInstance = nullptr;
    }

    DeActive();
}

void ATFT_NormalMonster_Rampage::UpdateBlackboardTarget()
{
}

void ATFT_NormalMonster_Rampage::PreActive()
{
    Super::PreActive();
}

void ATFT_NormalMonster_Rampage::Active()
{
    Super::Active();
}

void ATFT_NormalMonster_Rampage::DeActive()
{
    Super::DeActive();
    
    if (_animInstance_Rampage->IsValidLowLevel())
    {
        _animInstance_Rampage->OnMontageEnded.Clear();
        _animInstance_Rampage->_attackStartDelegate.Clear();
        _animInstance_Rampage->_attackHitDelegate.Clear();
        _animInstance_Rampage->_deathEndDelegate.Clear();
    }
}

void ATFT_NormalMonster_Rampage::SetAnimInstanceBind()
{
    Super::SetAnimInstanceBind();
    if (!bAnimBind)
    {
        _animInstance_Rampage = Cast<UTFT_AnimInstance_NormalRampage>(GetMesh()->GetAnimInstance());
        
        if (_animInstance_Rampage->IsValidLowLevel())
        {
            _animInstance_Rampage->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
            _animInstance_Rampage->_attackStartDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::AttackStart);
            _animInstance_Rampage->_attackHitDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::AttackHit_Boss);
            _animInstance_Rampage->_deathEndDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::BossDisable);
        
            bAnimBind = true;
        }
    }
}
