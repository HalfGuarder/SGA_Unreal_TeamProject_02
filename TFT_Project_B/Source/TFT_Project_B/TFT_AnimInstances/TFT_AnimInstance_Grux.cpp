// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimInstances/TFT_AnimInstance_Grux.h"

#include "TFT_Creature.h"

#include "GameFramework/PawnMovementComponent.h"

UTFT_AnimInstance_Grux::UTFT_AnimInstance_Grux()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> am
    (TEXT(""));

    if (am.Succeeded())
    {
        _attackMontage = am.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> sm
    (TEXT(""));

    if (sm.Succeeded())
    {
        _skillMontage = sm.Object;
    }
}

void UTFT_AnimInstance_Grux::NativeUpdateAnimation(float DeltaSeconds)
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

void UTFT_AnimInstance_Grux::PlayAttackMontage()
{
}

void UTFT_AnimInstance_Grux::PlaySkillMontage()
{
}

void UTFT_AnimInstance_Grux::JumpToSection(int32 sectionIndex)
{
    FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
    Montage_JumpToSection(sectionName);
}

void UTFT_AnimInstance_Grux::AnimNotify_AttackStart()
{
    _attackStartDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_AttackHit()
{
    _attackHitDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_DeathStart()
{
    _deathStartDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_DeathEnd()
{
    _deathEndDelegate.Broadcast();
}
