// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Creature.h"
#include "TFT_Characters/TFT_Monster.h"
#include "NiagaraSystem.h"
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
	virtual void AttackStart() override;
	virtual void FootStep() override;

	UFUNCTION()
	void AttackHit_Boss();

	virtual void Attack_AI();

	void MoveMessageForward();

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
	FTimerHandle MoveTimerHandle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_BJ* _animInstance_BJ;

protected:
	FVector TargetLocation;
	FTimerHandle SkillTimerHandle;
	FTimerHandle NiagaraEffectTimerHandle;
	FTimerHandle _deathTimerHandle;

	void SpawnNiagaraEffectAtLocation(FVector Location);
	void TriggerSkillEffect();

public:
	void ActivateSkillRandom();
	void ActivateSkill();
};
