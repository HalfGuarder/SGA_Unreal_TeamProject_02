// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimInstances/TFT_AnimInstance_Rampage.h"
#include "TFT_Characters/TFT_BossMonster_Rampage.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "TFT_Monster.h"



UTFT_AnimInstance_Rampage::UTFT_AnimInstance_Rampage()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> am
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_Rampage_Montager.TFT_Rampage_Montager'"));
    if (am.Succeeded())
    {
        _myAnimMontage = am.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> jm
    (TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_Rampage_JumpAttack_Montage.TFT_Rampage_JumpAttack_Montage'"));

    if (jm.Succeeded())
    {
       _JumpskillMontage = jm.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> sm
    (TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_Rampage_Fire.TFT_Rampage_Fire'"));

    if (sm.Succeeded())
    {
        _skillMontage = sm.Object;
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
       
        float playResult = Montage_Play(_myAnimMontage);

        
        if (playResult > 0.0f)
        {
           
        }
        else
        {
          
        }

      
        ATFT_Monster* myCharacter = Cast<ATFT_Monster>(TryGetPawnOwner());
        if (myCharacter)
        {
            
        }
        else
        {
           
        }
    }
}

void UTFT_AnimInstance_Rampage::PlayJumpSkillMontage()
{
    if (!Montage_IsPlaying(_JumpskillMontage))
    {
        Montage_Play(_JumpskillMontage);

        ATFT_Monster* myCharacter = Cast<ATFT_Monster>(TryGetPawnOwner());
        if (myCharacter)
        {
           
        }
        else
        {
           
        }
    }
}

void UTFT_AnimInstance_Rampage::PlaySkillMontage()
{
    if (!Montage_IsPlaying(_skillMontage))
    {
        Montage_Play(_skillMontage);

        ATFT_Monster* myCharacter = Cast<ATFT_Monster>(TryGetPawnOwner());
        if (myCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Skill Montage played with increased jump height!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Pawn Owner Cast Failed"));
        }
    }
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
