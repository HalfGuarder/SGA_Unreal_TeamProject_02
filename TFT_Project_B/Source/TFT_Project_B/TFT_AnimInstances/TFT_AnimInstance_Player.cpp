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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> fm
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Rifle_Attack_Montage.TFT_Player_Rifle_Attack_Montage'"));
	if (fm.Succeeded())
	{
		_fireMontage = fm.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> rm
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Running_AnimMontage.TFT_Player_Running_AnimMontage'"));
	if (rm.Succeeded())
	{
		_runningMontage = rm.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> rrm
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Rifle_Running_AnimMontage.TFT_Player_Rifle_Running_AnimMontage'"));
	if (rrm.Succeeded())
	{
		_rifleRunningMontage = rrm.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> dfs
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Defense_AnimMontage.TFT_Player_Defense_AnimMontage'"));
	if (dfs.Succeeded())
	{
		_defenseMontage = dfs.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> sd
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_ShieldDash_AnimMontage.TFT_Player_ShieldDash_AnimMontage'"));
	if (sd.Succeeded())
	{
		_shieldDashMontage = sd.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> sde
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_ShieldDash_End_AnimMontage.TFT_Player_ShieldDash_End_AnimMontage'"));
	if (sde.Succeeded())
	{
		_shieldDashEndMontage = sde.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> us
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_UpperSwing_AnimMontage.TFT_Player_UpperSwing_AnimMontage'"));
	if (us.Succeeded())
	{
		_upperSwingMontage = us.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> abrn
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Airborne_AnimMontage.TFT_Player_Airborne_AnimMontage'"));
	if (abrn.Succeeded())
	{
		_airborneMontage = abrn.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> stn
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Player/Animations/TFT_Player_Stun_AnimMontage.TFT_Player_Stun_AnimMontage'"));
	if (stn.Succeeded())
	{
		_stunMontage = stn.Object;
	}
}

void UTFT_AnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	ATFT_Creature* player = Cast<ATFT_Creature>(TryGetPawnOwner());
	if(player != nullptr)
	{
		bEquipSword = (player->bEquipSword);
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

		float pitch = player->GetBaseAimRotation().Pitch;
		if (pitch > -180.0f)
		{
			_pitch = pitch;
		}
		else
		{
			_pitch = GetWorld()->GetFirstPlayerController()->GetControlRotation().Pitch - 360.0f;
		}
	}
}

void UTFT_AnimInstance_Player::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UTFT_AnimInstance_Player::PlayAttackMontage()
{
	if (bEquipSword)
	{
		if (!Montage_IsPlaying(_attackMontage))
		{
			Montage_Play(_attackMontage);
		}
	}
	else
	{
		if (!Montage_IsPlaying(_fireMontage))
		{
			Montage_Play(_fireMontage);
		}
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

void UTFT_AnimInstance_Player::AnimNotify_AttackEnd()
{
	_attackEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_QSkillHit()
{
	// bIsShieldDashing = true;
	_qSkillHitDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_ESkillHit()
{
	_eSkillHitDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_ShieldDashCollisionOn()
{
	_ShieldDashCollisionOnDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::PlayRunningMontage()
{
	if (bEquipSword)
	{
		if (!Montage_IsPlaying(_runningMontage))
		{
			Montage_Play(_runningMontage, 1.0f);
		}
	}
	else
	{
		if (!Montage_IsPlaying(_rifleRunningMontage))
		{
			Montage_Play(_rifleRunningMontage, 1.0f);
		}
	}
}

void UTFT_AnimInstance_Player::StopRunningMontage()
{
	if (bEquipSword)
	{
		//if (Montage_IsPlaying(_runningMontage))
		//{
			Montage_Stop(0.2f, _runningMontage);
		//}
	}
	else
	{
		Montage_Stop(0.2f, _rifleRunningMontage);
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
	if (Montage_IsPlaying(_shieldDashMontage))
	{
		Montage_Stop(0.2f, _shieldDashMontage);

		Montage_Play(_shieldDashEndMontage);

		bIsShieldDashing = false;
		_shieldDashEndDelegate.Broadcast();
	}
}

void UTFT_AnimInstance_Player::PlayUpperSwingMontage()
{
	if (!Montage_IsPlaying(_upperSwingMontage))
	{
		Montage_Play(_upperSwingMontage);
	}
}

void UTFT_AnimInstance_Player::StopUpperSwingMontage()
{

}

void UTFT_AnimInstance_Player::PlayDefenseMontage()
{
	if (!Montage_IsPlaying(_defenseMontage))
	{
		Montage_Play(_defenseMontage);
	}
}

void UTFT_AnimInstance_Player::StopDefenseMontage()
{
	bIsDefensing = false;
	Montage_Stop(0.2f, _defenseMontage);
}

void UTFT_AnimInstance_Player::PlayAirborneMontage()
{
	if (!Montage_IsPlaying(_airborneMontage))
	{
		Montage_Play(_airborneMontage);
	}
}

void UTFT_AnimInstance_Player::StopAirborneMontage()
{
}

void UTFT_AnimInstance_Player::PlayStunMontage()
{
	if (!Montage_IsPlaying(_stunMontage))
	{
		Montage_Play(_stunMontage);
	}
}

void UTFT_AnimInstance_Player::StopStunMontage()
{
}

void UTFT_AnimInstance_Player::AnimNotify_ShieldDashEnd()
{
	bIsShieldDashing = false;
	_shieldDashEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_Fire()
{
	_fireDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_DeathStart()
{
	_DeathDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_AirborneEnd()
{
	_stateMontageEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_StunEnd()
{
	_stateMontageEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Player::AnimNotify_FootStep()
{
	_footStepDelegate.Broadcast();
}
