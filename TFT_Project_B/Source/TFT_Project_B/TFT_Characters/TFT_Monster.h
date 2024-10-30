// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Creature.h"
#include "TFT_Monster.generated.h"



UCLASS(Abstract)
class TFT_PROJECT_B_API ATFT_Monster : public ATFT_Creature
{
	GENERATED_BODY()

public:
	ATFT_Monster();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Attack_AI();

	UFUNCTION()
	virtual void AttackEnd();

	UFUNCTION()
	virtual void DropItem();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


};
