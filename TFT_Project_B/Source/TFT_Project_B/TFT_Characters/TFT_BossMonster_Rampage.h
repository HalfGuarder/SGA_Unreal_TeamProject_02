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
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetMesh(FString path) override;

	UFUNCTION()
	void AttackHit_Boss();


	virtual void Attack_AI();
	void ResetSkillCooldown();
	void ResetJumpSkillCooldown();
	
	virtual void AttackEnd();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void DeathStart() override;

	UFUNCTION()
	void ResetMovementLock(UAnimMontage* Montage, bool bInterrupted); 

	UFUNCTION()
	void BossDisable();

	void UpdateBlackboardTarget();

	UFUNCTION()
	void ExplosionHit(); // Explosion 공격

	UFUNCTION()
	void ChainExplosionHit(); // Chain Explosion 공격

	void Active() override;

	void DeActive() override;

	void SetAnimInstanceBind() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_Rampage* _animInstance_Rampage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* armcapsule_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* armcapsule_L;

	
	bool bIsDead = false;
	
	APlayerController* PlayerController;

	FVector LockedLocation; 

	float SkillCooldown = 15.0f;
	bool bCanUseSkill = true;
	FTimerHandle SkillCooldownTimerHandle;

	float JumpSkillCooldown = 10.0f;
	bool bCanUseJumpSkill = true;
	FTimerHandle JumpSkillCooldownTimerHandle;

	FTimerHandle _deathTimerHandle;
};
