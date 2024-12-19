// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_NormalMonster_Rampage.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_NormalMonster_Rampage : public ATFT_Monster
{
	GENERATED_BODY()
public:
	ATFT_NormalMonster_Rampage();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetMesh(FString path) override;

	UFUNCTION()
	void AttackHit_Boss();

	virtual void Attack_AI();

	virtual void AttackEnd();

	// virtual void DropItem();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void DeathStart() override;

	UFUNCTION()
	void ResetMovementLock(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void BossDisable();

	void UpdateBlackboardTarget();

	void PreActive() override;

	void Active() override;

	void DeActive() override;

	void SetAnimInstanceBind() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_NormalRampage* _animInstance_Rampage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* armcapsule_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* armcapsule_L;

	bool bIsDead = false;

	APlayerController* PlayerController;

	FVector LockedLocation;

	FTimerHandle _deathTimerHandle;
};
