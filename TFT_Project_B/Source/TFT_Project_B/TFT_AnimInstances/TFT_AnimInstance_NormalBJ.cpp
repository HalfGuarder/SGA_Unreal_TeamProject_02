// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimInstances/TFT_AnimInstance_NormalBJ.h"
#include "TFT_Characters/TFT_Boss_BJ.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

UTFT_AnimInstance_NormalBJ::UTFT_AnimInstance_NormalBJ()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> bm(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animation/Boss_Animation/Animation/NEW_BOSS_ATTACKBoss_Attack.NEW_BOSS_ATTACKBoss_Attack'"));
	if (bm.Succeeded())
	{
		_attackMontage = bm.Object;
	}
}

void UTFT_AnimInstance_NormalBJ::NativeUpdateAnimation(float DeltaSeconds)
{
	ATFT_Creature* myCharacter = Cast<ATFT_Creature>(TryGetPawnOwner());

	if (myCharacter != nullptr)
	{
		_speed = myCharacter->GetVelocity().Size();
		_isfalling = myCharacter->GetMovementComponent()->IsFalling();
		_vertical = _vertical + (myCharacter->_vertical - _vertical) * DeltaSeconds;
		_horizontal = _horizontal + (myCharacter->_horizontal - _horizontal) * DeltaSeconds;
		_isdead = (myCharacter->GetCurHp() <= 0);
	}
}

void UTFT_AnimInstance_NormalBJ::PlayAttackMontage()
{
	if (!Montage_IsPlaying(_myAnimMontage))
	{
		Montage_Play(_myAnimMontage);

		ATFT_Monster* myCharacter = Cast<ATFT_Monster>(TryGetPawnOwner());
		if (myCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn Owner Cast Successful"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Pawn Owner Cast Failed"));
		}

	}
}

void UTFT_AnimInstance_NormalBJ::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UTFT_AnimInstance_NormalBJ::AnimNotify_AttackStart()
{
	_attackStartDelegate.Broadcast();
}

void UTFT_AnimInstance_NormalBJ::AnimNotify_AttackHit()
{
	_attackHitDelegate.Broadcast();
}

void UTFT_AnimInstance_NormalBJ::AnimNotify_DeathStart()
{
	_deathStartDelegate.Broadcast();
}

void UTFT_AnimInstance_NormalBJ::AnimNotify_DeathEnd()
{
	_deathEndDelegate.Broadcast();
}
