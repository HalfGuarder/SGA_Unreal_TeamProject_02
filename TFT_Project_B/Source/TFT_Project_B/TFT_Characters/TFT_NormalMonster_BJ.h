// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_NormalMonster_BJ.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_NormalMonster_BJ : public ATFT_Monster
{
	GENERATED_BODY()
	
public:
	ATFT_NormalMonster_BJ();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetMesh(FString path) override;
	virtual void AttackStart() override;
	
	UFUNCTION()
	void AttackHit_Boss();

	virtual void Attack_AI();

	virtual void AttackEnd();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void DeathStart() override;

	UFUNCTION()
	void BossDisable();

	void SetAnimInstanceBind() override;

	void PreActive() override;

	void Active() override;

	void DeActive() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_NormalBJ* _animInstance_BJ;

	FTimerHandle _deathTimerHandle;


};
