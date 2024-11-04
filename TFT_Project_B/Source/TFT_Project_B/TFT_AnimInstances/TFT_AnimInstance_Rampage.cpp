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

void UTFT_AnimInstance_Rampage::PlayAttackMontage()
{
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
