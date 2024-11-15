// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TFT_AnimInstance_Rampage.generated.h"

DECLARE_MULTICAST_DELEGATE(AttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(AttackHitDelegate);
DECLARE_MULTICAST_DELEGATE(DeathStartDelegate);
DECLARE_MULTICAST_DELEGATE(DeathEndDelegate);
DECLARE_MULTICAST_DELEGATE(BossDeathEndDelegate);


UCLASS()
class TFT_PROJECT_B_API UTFT_AnimInstance_Rampage : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTFT_AnimInstance_Rampage();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlaySkillMontage();

	void JumpToSection(int32 sectionIndex);

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_DeathStart();

	UFUNCTION()
	void AnimNotify_DeathEnd();

	UFUNCTION()
	void AnimNotify_Boss_DeathEnd();

	AttackStartDelegate _attackStartDelegate;
	AttackHitDelegate _attackHitDelegate;
	DeathStartDelegate _deathStartDelegate;
	DeathEndDelegate _deathEndDelegate;
	BossDeathEndDelegate _bossDeathEndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _myAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _skillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _skillMontage2;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool _isFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool _isDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _vertical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _attackMontage;
};
