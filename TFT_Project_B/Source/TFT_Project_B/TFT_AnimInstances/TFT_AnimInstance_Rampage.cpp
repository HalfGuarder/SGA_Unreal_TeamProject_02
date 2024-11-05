// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimInstances/TFT_AnimInstance_Rampage.h"
#include "TFT_Characters/TFT_BossMonster_Rampage.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"



UTFT_AnimInstance_Rampage::UTFT_AnimInstance_Rampage()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> am
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_Rampage_Montager.TFT_Rampage_Montager'"));
	if (am.Succeeded())
	{
		_attackMontage = am.Object;
	}



}

void UTFT_AnimInstance_Rampage::NativeUpdateAnimation(float DeltaSeconds)
{
	ATFT_Creature* myCharacter = Cast<ATFT_Creature>(TryGetPawnOwner());

	if (myCharacter != nullptr)
	{
		_speed = myCharacter->GetVelocity().Size();
		_isFalling = myCharacter->GetMovementComponent()->IsFalling();
		_vertical = _vertical + (myCharacter->_vertical - _vertical) * DeltaSeconds;
		_horizontal = _horizontal + (myCharacter->_horizontal - _horizontal) * DeltaSeconds;
		_isDead = (myCharacter->GetCurHp() <= 0);
	}
}

void UTFT_AnimInstance_Rampage::PlayAttackMontage()
{
	if (!Montage_IsPlaying(_myAnimMontage))
	{
		Montage_Play(_myAnimMontage);

		ATFT_Monster* myCharacter = Cast<ATFT_Monster>(TryGetPawnOwner());
	}
}

void UTFT_AnimInstance_Rampage::PlaySkillMontage()
{
}

void UTFT_AnimInstance_Rampage::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UTFT_AnimInstance_Rampage::AnimNotify_AttackStart()
{
	_attackStartDelegate.Broadcast();
}

void UTFT_AnimInstance_Rampage::AnimNotify_AttackHit()
{
	_attackHitDelegate.Broadcast();
}

void UTFT_AnimInstance_Rampage::AnimNotify_DeathStart()
{
	_deathStartDelegate.Broadcast();
}

void UTFT_AnimInstance_Rampage::AnimNotify_DeathEnd()
{
	_deathEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Rampage::AnimNotify_Boss_DeathEnd()
{
	_bossDeathEndDelegate.Broadcast();
}
