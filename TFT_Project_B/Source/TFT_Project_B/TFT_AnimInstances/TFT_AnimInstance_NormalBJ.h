// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TFT_AnimInstance_NormalBJ.generated.h"

DECLARE_MULTICAST_DELEGATE(BJNormalAnimEvent);

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_AnimInstance_NormalBJ : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTFT_AnimInstance_NormalBJ();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	void PlayDeathMontage();

	void JumpToSection(int32 sectionIndex);

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_DeathStart();

	UFUNCTION()
	void AnimNotify_DeathEnd();


	BJNormalAnimEvent _attackStartDelegate;
	BJNormalAnimEvent _attackHitDelegate;
	BJNormalAnimEvent _deathStartDelegate;
	BJNormalAnimEvent _deathEndDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _attackMontage;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool _isfalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool _isdead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _vertical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* _deathMontage;
};
