// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_BossMonster_Rampage.h"
#include "TFT_AnimInstances/TFT_AnimInstance_Rampage.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"
#include "TFT_Characters/TFT_Player.h"

#include "Engine/DamageEvents.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

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

    HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBarWidgetComponent"));
    HpBarWidgetComponent->SetupAttachment(RootComponent);
    HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);  
     
    if (HpBarWidgetClass)
    {
        HpBarWidgetComponent->SetWidgetClass(HpBarWidgetClass);  
    }
}

void ATFT_BossMonster_Rampage::BeginPlay()
{
    Super::BeginPlay();

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
        _animInstance_Boss->_bossDeathEndDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::Boss_DeathEnd);
        _animInstance_Boss->_deathEndDelegate.AddUObject(this, &ATFT_BossMonster_Rampage::BossDisable);
    }

    if (HpBarWidgetComponent)
    {
       
        HpBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 250));

        
        if (HpBarWidgetComponent->GetWidget())
        {
            UTFT_HPBarWidget* HpBar = Cast<UTFT_HPBarWidget>(HpBarWidgetComponent->GetWidget());
            if (HpBar)
            {
                _statCom->_hpChangedDelegate.AddUObject(HpBar, &UTFT_HPBarWidget::SetHpBarValue);
            }
        }
    }
}

void ATFT_BossMonster_Rampage::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerController && HpBarWidgetComponent)
    {
        
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            
            float DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());

            
            if (DistanceToPlayer <= HpBarVisibilityDistance)
            {
                HpBarWidgetComponent->SetVisibility(true);
            }
            else
            {
                HpBarWidgetComponent->SetVisibility(false);
            }
        }
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
        ECollisionChannel::ECC_GameTraceChannel10,
        FCollisionShape::MakeSphere(attackRadius),
        params
    );

    FVector vec = GetActorForwardVector() * attackRange;
    FVector center = GetActorLocation() + vec * 0.5f;

    FColor drawColor = FColor::Green;

    if (bResult && hitResult.GetActor()->IsValidLowLevel())
    {
        float hpRatio = _statCom->HpRatio();
        float damageMultiplier = (hpRatio < 0.3f) ? 2.0f : 1.0f;

        float baseDamage = _statCom->GetAttackDamage();
        float damage = baseDamage * damageMultiplier;

        FDamageEvent damageEvent;
        hitResult.GetActor()->TakeDamage(damage, damageEvent, GetController(), this);
        _hitPoint = hitResult.ImpactPoint;
        drawColor = FColor::Red;

        //EffectManager->Play("N_Monster_Boss_Attack_Hit", 1, _hitPoint);


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
            !_animInstance_Boss->Montage_IsPlaying(_animInstance_Boss->_skillMontage))
        {
            _animInstance_Boss->PlayAttackMontage();

            _isAttacking = true;

            _curAttackIndex %= 3;
            _curAttackIndex++;
            _animInstance_Boss->JumpToSection(_curAttackIndex);
        }
    }
}

void ATFT_BossMonster_Rampage::AttackEnd()
{
}

void ATFT_BossMonster_Rampage::DropItem()
{
}

float ATFT_BossMonster_Rampage::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ATFT_BossMonster_Rampage::DeathStart()
{
}

void ATFT_BossMonster_Rampage::Boss_DeathEnd()
{
    this->SetActorHiddenInGame(true);

    _animInstance_Boss->_bossDeathEndDelegate.RemoveAll(this);
}

void ATFT_BossMonster_Rampage::BossDisable()
{
}

void ATFT_BossMonster_Rampage::UpdateBlackboardTarget()
{
}
