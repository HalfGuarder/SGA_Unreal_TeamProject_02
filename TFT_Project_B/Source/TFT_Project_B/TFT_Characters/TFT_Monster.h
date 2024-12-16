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
	virtual void DropItem(MonsterType type);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage_BP(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void ChangeMesh(TObjectPtr<USkeletalMesh> mesh) override;

	virtual void DeathStart() override;

	virtual void PreActive();

	virtual void Active();

	virtual void DeActive();

	virtual void SetAnimInstanceBind();

	virtual void ChangeLevel(int32 level);

	void ReStartBT(AActor* target);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Spawn, meta = (AllowPrivateAccess = true))
	bool bIsSpawned = false;

	bool bAnimBind = false;
	bool bItemDroped = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class ATFT_Boss_AIController* _controller = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EXP, meta = (AllowPrivateAccess = true))
	int32 _possessionExp;
};
