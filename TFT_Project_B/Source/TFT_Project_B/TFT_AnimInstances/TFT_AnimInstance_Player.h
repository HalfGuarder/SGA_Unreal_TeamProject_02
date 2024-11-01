// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TFT_AnimInstance_Player.generated.h"

DECLARE_MULTICAST_DELEGATE(AttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(AttackHitDelegate);
DECLARE_MULTICAST_DELEGATE(AttackHit_QDelegate);
DECLARE_MULTICAST_DELEGATE(DashEndDelegate);

UCLASS()
class TFT_PROJECT_B_API UTFT_AnimInstance_Player : public UAnimInstance
{
	GENERATED_BODY()

public:
	UTFT_AnimInstance_Player();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void JumpToSection(int32 sectionIndex);

	void PlayAttackMontage();

	UFUNCTION()
	void AnimNotify_DashEnd();

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackHit();

	void PlayRunningMontage();
	void StopRunningMontage();

	AttackStartDelegate _attackStartDelegate;
	AttackHitDelegate _attackHitDelegate;
	DashEndDelegate _dashEndDelegate;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bIsDashing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bFDashing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bBDashing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bLDashing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bRDashing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _vertical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _attackMontage;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement , Meta = (AllowPrivateAccess = true))
	// UAnimMontage* _runningMontage;
};
