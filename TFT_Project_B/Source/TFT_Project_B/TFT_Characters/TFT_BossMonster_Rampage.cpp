// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_BossMonster_Rampage.h"
#include "TFT_AnimInstances/TFT_AnimInstance_Rampage.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ATFT_BossMonster_Rampage::ATFT_BossMonster_Rampage()
{
	_meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));

	SetMesh("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");


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
}

void ATFT_BossMonster_Rampage::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATFT_BossMonster_Rampage::SetMesh(FString path)
{
	_meshCom->SetMesh(path);
}

void ATFT_BossMonster_Rampage::AttackHit_Boss()
{
}

void ATFT_BossMonster_Rampage::Attack_AI()
{
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
