// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimInstance_Player.h"
#include "TFT_Creature.h"
#include "GameFramework/PawnMovementComponent.h"
// #include "Animation/AnimMontage.h"

UTFT_AnimInstance_Player::UTFT_AnimInstance_Player()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> am
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Sword_Attack_AnimMontage.TFT_Player_Sword_Attack_AnimMontage'"));
	if (am.Succeeded())
	{
		_attackMontage = am.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> rm
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Running_AnimMontage.TFT_Player_Running_AnimMontage'"));
	if (rm.Succeeded())
	{
		_runningMontage = rm.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> sd
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/Skill_Test/Use/TFT_Player_ShieldDash_AnimMontage.TFT_Player_ShieldDash_AnimMontage'"));
	if (sd.Succeeded())
	{
		_shieldDashMontage = sd.Object;
	}


}

void UTFT_AnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	ATFT_Creature* player = Cast<ATFT_Creature>(TryGetPawnOwner());
	if(player != nullptr)
	{
		_speed = player->GetVelocity().Size();
		bIsFalling = player->GetMovementComponent()->IsFalling();
		_vertical = _vertical + (player->_vertical - _vertical) * DeltaSeconds;
		_horizontal = _horizontal + (player->_horizontal - _horizontal) * DeltaSeconds;
		bIsDead = (player->GetCurHp() <= 0);
		bIsDashing = (player->bIsDashing);
		bFDashing = (player->bFDashing);
		bBDashing = (player->bBDashing);
		bLDashing = (player->bLDashing);
		bRDashing = (player->bRDashing);
	}
}

void UTFT_AnimInstance_Player::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UTFT_AnimInstance_Player::PlayAttackMontage()
{
	if (!Montage_IsPlaying(_attackMontage))
	{
		Montage_Play(_attackMontage);
	}
}

void UTFT_AnimInstance_Player::AnimNotify_DashEnd()
{
	_dashEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_AttackStart()
{
	_attackStartDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_AttackHit()
{
	_attackHitDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_QSkillHit()
{
	bIsShieldDashing = true;
	_qSkillHitDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::PlayRunningMontage()
{
	if (!Montage_IsPlaying(_runningMontage))
	{
		Montage_Play(_runningMontage, 1.0f);
	}
}

void UTFT_AnimInstance_Player::StopRunningMontage()
{
	if (Montage_IsPlaying(_runningMontage))
	{
		Montage_Stop(0.2f, _runningMontage);
	}
}

void UTFT_AnimInstance_Player::PlayShieldDashMontage()
{
	if (!Montage_IsPlaying(_shieldDashMontage))
	{
		Montage_Play(_shieldDashMontage);
	}
}

void UTFT_AnimInstance_Player::StopShiedlDashMontage()
{

}

void UTFT_AnimInstance_Player::AnimNotify_ShieldDashEnd()
{
	bIsShieldDashing = false;
}
