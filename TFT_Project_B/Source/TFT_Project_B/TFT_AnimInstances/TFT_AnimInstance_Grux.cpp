// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AnimInstances/TFT_AnimInstance_Grux.h"

#include "TFT_Creature.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

UTFT_AnimInstance_Grux::UTFT_AnimInstance_Grux()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> am
    (TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_Grux_Attack_AnimMontage.TFT_Grux_Attack_AnimMontage'"));

    if (am.Succeeded())
    {
        _attackMontage = am.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> sm
    (TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_Grux_Skill_AnimMontage.TFT_Grux_Skill_AnimMontage'"));

    if (sm.Succeeded())
    {
        _skillMontage = sm.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> stn
    (TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_Grux_Stun_AnimMontage.TFT_Grux_Stun_AnimMontage'"));

    if (stn.Succeeded())
    {
        _stunMontage = stn.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> abrn
    (TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_Grux_Airborne_AnimMontage.TFT_Grux_Airborne_AnimMontage'"));

    if (abrn.Succeeded())
    {
        _airborneMontage = abrn.Object;
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
    if (!Montage_IsPlaying(_attackMontage))
    {
        Montage_Play(_attackMontage);
    }
}

void UTFT_AnimInstance_Grux::PlaySkillMontage()
{
    if (!Montage_IsPlaying(_skillMontage))
    {
        Montage_Play(_skillMontage);
    }
}

void UTFT_AnimInstance_Grux::PlayAirborneMontage()
{
    if (!Montage_IsPlaying(_airborneMontage))
    {
        Montage_Play(_airborneMontage);
    }
}

void UTFT_AnimInstance_Grux::StopAirborneMontage()
{

}

void UTFT_AnimInstance_Grux::PlayStunMontage()
{
    if (!Montage_IsPlaying(_stunMontage))
    {
        Montage_Play(_stunMontage);
    }
}

void UTFT_AnimInstance_Grux::StopStunMontage()
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

void UTFT_AnimInstance_Grux::AnimNotify_AttackEnd()
{
    _attackEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_DeathStart()
{
    _deathStartDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_DeathEnd()
{
    _deathEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_AirborneEnd()
{
    _stateMontageEndDelegate.Broadcast();
}

void UTFT_AnimInstance_Grux::AnimNotify_StunEnd()
{
    _stateMontageEndDelegate.Broadcast();
}
