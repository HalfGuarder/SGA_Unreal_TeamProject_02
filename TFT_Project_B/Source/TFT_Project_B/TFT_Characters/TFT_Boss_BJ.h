// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Creature.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_Boss_BJ.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_Boss_BJ : public ATFT_Monster
{
	GENERATED_BODY()
	
public:
	ATFT_Boss_BJ();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetMesh(FString path) override;

	UFUNCTION()
	void AttackHit_Boss();

	virtual void Attack_AI();


	virtual void AttackEnd();


	virtual void DropItem();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void DeathStart() override;

	UFUNCTION()
	void Boss_DeathEnd();

	UFUNCTION()
	void BossDisable();

private:
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_BJ* _animInstance_BJ;
};
