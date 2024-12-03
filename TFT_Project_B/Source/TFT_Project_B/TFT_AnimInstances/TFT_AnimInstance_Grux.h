// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TFT_AnimInstance_Grux.generated.h"

DECLARE_MULTICAST_DELEGATE(GruxAnimEvent);
//DECLARE_MULTICAST_DELEGATE(AttackHitDelegate);
//DECLARE_MULTICAST_DELEGATE(AttackEndDelegate);
//DECLARE_MULTICAST_DELEGATE(DeathStartDelegate);
//DECLARE_MULTICAST_DELEGATE(DeathEndDelegate);

UCLASS()
class TFT_PROJECT_B_API UTFT_AnimInstance_Grux : public UAnimInstance
{
	GENERATED_BODY()

public:
	UTFT_AnimInstance_Grux();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	void PlaySkillMontage();

	void PlayAirborneMontage();
	void StopAirborneMontage();

	void PlayStunMontage();
	void StopStunMontage();

	void JumpToSection(int32 sectionIndex);

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DeathStart();

	UFUNCTION()
	void AnimNotify_DeathEnd();

	UFUNCTION()
	void AnimNotify_AirborneEnd();

	UFUNCTION()
	void AnimNotify_StunEnd();
	
	GruxAnimEvent _attackStartDelegate;
	GruxAnimEvent _attackHitDelegate;
	GruxAnimEvent _attackEndDelegate;
	GruxAnimEvent _deathStartDelegate;
	GruxAnimEvent _deathEndDelegate;
	GruxAnimEvent _stateMontageEndDelegate;

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _attackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _skillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _stunMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _airborneMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDashing = false;
};
