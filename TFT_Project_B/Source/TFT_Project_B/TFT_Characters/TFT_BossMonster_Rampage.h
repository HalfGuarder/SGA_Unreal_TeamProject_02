// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_BossMonster_Rampage.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_BossMonster_Rampage : public ATFT_Monster
{
	GENERATED_BODY()
public:
	ATFT_BossMonster_Rampage();

	virtual void BeginPlay() override;

	
	virtual void Attack_AI();

	
	virtual void AttackEnd();

	
	virtual void DropItem();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
