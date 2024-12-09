// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_NormalMonster_Rampage.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "TFT_Characters/TFT_Player.h"
#include "../TFT_Components/TFT_StatComponent.h"
#include "TFT_AnimInstances/TFT_AnimInstance_NormalRampage.h"

#include "Engine/DamageEvents.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h" 

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ATFT_NormalMonster_Rampage::ATFT_NormalMonster_Rampage()
{
    _meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));

    SetMesh("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'");

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

void ATFT_NormalMonster_Rampage::BeginPlay()
{
    Super::BeginPlay();

    _statCom->SetLevelAndInit(1);
    PlayerController = GetWorld()->GetFirstPlayerController();
}

void ATFT_NormalMonster_Rampage::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    _animInstance_Boss = Cast<UTFT_AnimInstance_NormalRampage>(GetMesh()->GetAnimInstance());
    if (_animInstance_Boss->IsValidLowLevel())
    {
        _animInstance_Boss->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
        _animInstance_Boss->_attackStartDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::AttackStart);
        _animInstance_Boss->_attackHitDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::AttackHit_Boss);
        _animInstance_Boss->_deathStartDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::DeathStart);
        _animInstance_Boss->_deathEndDelegate.AddUObject(this, &ATFT_NormalMonster_Rampage::BossDisable);
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

void ATFT_NormalMonster_Rampage::Attack_AI()
{
    if (!_isAttacking && _animInstance_Boss != nullptr)
    {
        // �ٸ� ��Ÿ�ְ� ���� ������ ���� ���� ����
        if (!_animInstance_Boss->Montage_IsPlaying(_animInstance_Boss->_myAnimMontage))
        {
            // ���� ��ġ�� ��� ó��
            LockedLocation = GetActorLocation();

            // �Ϲ� ���� ��Ÿ�� ���
            _animInstance_Boss->PlayAttackMontage();

            _isAttacking = true;

            // ���� ���� ��ȯ ó��
            _curAttackIndex %= 3;
            _curAttackIndex++;
            _animInstance_Boss->JumpToSection(_curAttackIndex);

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

    _animInstance_Boss->_deathStartDelegate.RemoveAll(this);
}

void ATFT_NormalMonster_Rampage::ResetMovementLock(UAnimMontage* Montage, bool bInterrupted)
{
    _isAttacking = false;

    _animInstance_Boss->OnMontageEnded.RemoveDynamic(this, &ATFT_NormalMonster_Rampage::ResetMovementLock);
}

void ATFT_NormalMonster_Rampage::BossDisable()
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

void ATFT_NormalMonster_Rampage::UpdateBlackboardTarget()
{
}
