// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TFT_AnimInstance_Player.generated.h"

DECLARE_MULTICAST_DELEGATE(AttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(AttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(AttackHitDelegate);
DECLARE_MULTICAST_DELEGATE(QSkillHitDelegate);
DECLARE_MULTICAST_DELEGATE(ESkillHitDelegate);
DECLARE_MULTICAST_DELEGATE(ShieldDashCollisionOnDelegate);
DECLARE_MULTICAST_DELEGATE(DashEndDelegate);
DECLARE_MULTICAST_DELEGATE(ShieldDashEndDelegate);
DECLARE_MULTICAST_DELEGATE(FireDelegate);

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

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_QSkillHit();

	UFUNCTION()
	void AnimNotify_ESkillHit();

	UFUNCTION()
	void AnimNotify_ShieldDashCollisionOn();

	UFUNCTION()
	void AnimNotify_ShieldDashEnd();

	UFUNCTION()
	void AnimNotify_Fire();

	void PlayRunningMontage();
	void StopRunningMontage();

	void PlayShieldDashMontage();
	void StopShiedlDashMontage();

	void PlayUpperSwingMontage();
	void StopUpperSwingMontage();

	void PlayDefenseMontage();
	void StopDefenseMontage();


	AttackStartDelegate _attackStartDelegate;
	AttackHitDelegate _attackHitDelegate;
	AttackEndDelegate _attackEndDelegate;
	ShieldDashCollisionOnDelegate _ShieldDashCollisionOnDelegate;
	DashEndDelegate _dashEndDelegate;
	QSkillHitDelegate _qSkillHitDelegate;
	ESkillHitDelegate _eSkillHitDelegate;
	ShieldDashEndDelegate _shieldDashEndDelegate;
	FireDelegate _fireDelegate;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bEquipSword;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _attackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _fireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement , Meta = (AllowPrivateAccess = true))
	UAnimMontage* _runningMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _rifleRunningMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defense, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _defenseMontage;

	// Skills
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _shieldDashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _shieldDashEndMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _upperSwingMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	bool bIsShieldDashing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Defense, Meta = (AllowPrivateAccess = true))
	bool bIsDefensing = false;

};
