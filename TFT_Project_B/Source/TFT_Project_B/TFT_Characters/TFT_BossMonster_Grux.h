// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_BossMonster_Grux.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_BossMonster_Grux : public ATFT_Monster
{
	GENERATED_BODY()
	
public:
	ATFT_BossMonster_Grux();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetMesh(FString path) override;

	UFUNCTION()
	void AttackHit_Boss();

	virtual void Attack_AI();

	virtual void AttackEnd();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void DeathStart() override;

	UFUNCTION()
	void BossDisable();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_Grux* _animInstance_Grux;

	FVector LockedLocation;
	FRotator LockedRotation;
};
