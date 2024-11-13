// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Creature.h"

#include "Engine/DamageEvents.h"
#include "Math/UnrealMathUtility.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "TFT_PlayerController.h"

#include "TFT_GameInstance.h"
#include "TFT_InvenComponent.h"
#include "TFT_StatComponent.h"
#include "TFT_StateComponent.h"

#include "TFT_Item.h"
#include "TFT_HPBarWidget.h"

ATFT_Creature::ATFT_Creature()
{
	PrimaryActorTick.bCanEverTick = true;

	_statCom = CreateDefaultSubobject<UTFT_StatComponent>(TEXT("Stat_Com"));
	_stateCom = CreateDefaultSubobject<UTFT_StateComponent>(TEXT("State_Com"));

}

void ATFT_Creature::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATFT_Creature::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATFT_Creature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_Creature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATFT_Creature::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bUseControllerRotationYaw = true;
	_isAttacking = false;
	_canMove = true;
	_attackEndedDelegate.Broadcast();
}

float ATFT_Creature::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	float damaged = -(_statCom->AddCurHp(-Damage));
	

	return damaged;
}

void ATFT_Creature::AttackStart()
{
}

void ATFT_Creature::DeathStart()
{
	this->SetActorEnableCollision(false);
	PrimaryActorTick.bCanEverTick = false;
	auto controller = GetController();
	if (controller != nullptr) GetController()->UnPossess();
}

void ATFT_Creature::Disable()
{
	this->SetActorHiddenInGame(true);
}

void ATFT_Creature::FootStep()
{
}

