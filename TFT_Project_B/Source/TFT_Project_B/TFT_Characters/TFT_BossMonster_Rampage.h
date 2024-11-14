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

	UFUNCTION()
	void OnArmCapsuleHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Attack_AI();
	void ResetSkillCooldown();
	
	virtual void AttackEnd();

	
	virtual void DropItem();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void DeathStart() override;

	UFUNCTION()
	void ResetMovementLock(UAnimMontage* Montage, bool bInterrupted); 


	UFUNCTION()
	void Boss_DeathEnd();

	UFUNCTION()
	void BossDisable();

	void UpdateBlackboardTarget();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_Rampage* _animInstance_Boss;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* armcapsule_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* armcapsule_L;

	UPROPERTY(EditAnywhere, Category = "Components")
	UWidgetComponent* HpBarWidgetComponent; 

	UPROPERTY(EditAnywhere, Category = "HP Bar")
	float HpBarVisibilityDistance = 500.0f; 

	
	APlayerController* PlayerController;

	FVector LockedLocation; 

	float SkillCooldown = 10.0f;
	bool bCanUseSkill = true;
	FTimerHandle SkillCooldownTimerHandle;
};
