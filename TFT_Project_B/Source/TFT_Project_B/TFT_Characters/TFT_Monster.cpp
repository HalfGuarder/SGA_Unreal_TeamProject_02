// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Monster.h"
#include "TFT_Player.h"

#include "Engine/DamageEvents.h"

ATFT_Monster::ATFT_Monster()
{
	_invenCom = CreateDefaultSubobject<UTFT_InvenComponent>(TEXT("Inven_Com"));
	_meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("MeshComponent"));
}

void ATFT_Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATFT_Monster::BeginPlay()
{
	Super::BeginPlay();
}

void ATFT_Monster::Attack_AI()
{
}

void ATFT_Monster::AttackEnd()
{
}

void ATFT_Monster::DropItem(MonsterType type)
{
	if (IsValid(this)) // �Ǵ� IsValid(GetWorld())
	{
		FVector monsterPos = GetActorLocation();
		_invenCom->DropMonsterItem(monsterPos, type);
	}
	//_invenCom->DropMonsterItem(monsterPos);
}

float ATFT_Monster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return damage;
}

void ATFT_Monster::TakeDamage_BP(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ATFT_Monster::ChangeMesh(TObjectPtr<USkeletalMesh> mesh)
{
	_meshCom->ChangeMesh(mesh);
}
