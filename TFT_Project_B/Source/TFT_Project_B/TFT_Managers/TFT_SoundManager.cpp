// Fill out your copyright notice in the Description page of Project Settings.

#include "TFT_SoundManager.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

ATFT_SoundManager::ATFT_SoundManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateSoundCue("Explosion", "/Script/Engine.SoundCue'/Game/Blueprints/SFX/Rampage_Sound/explosion.explosion'");

	// Player
	// Sword
	CreateSoundCue("P_ChangeWeapon_Sword", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_SetWeapon_Sword_SQ.TFT_Player_SetWeapon_Sword_SQ'");
	CreateSoundCue("P_Sword_Swing", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_Sword_Swing_SQ.TFT_Player_Sword_Swing_SQ'");
	CreateSoundCue("P_Shield_On", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_ShieldOn_SQ.TFT_Player_ShieldOn_SQ'");
	CreateSoundCue("P_Shield_Off", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_ShieldOff_SQ.TFT_Player_ShieldOff_SQ'");
	CreateSoundCue("P_Shield_Hit", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_ShieldHit_SQ.TFT_Player_ShieldHit_SQ'");
	CreateSoundCue("P_EskillSound", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/Sound/PlayerSlash.PlayerSlash'");
	// Rifle
	CreateSoundCue("P_ChangeWeapon_Rifle", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_SetWeapon_Rifle_SQ.TFT_Player_SetWeapon_Rifle_SQ'");
	CreateSoundCue("P_Rifle_Fire", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_Rifle_Fire_SQ.TFT_Player_Rifle_Fire_SQ'");
	// Skill
	CreateSoundCue("P_Rifle_QSkill_LaserHit", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_LaserHit_SQ.TFT_Player_LaserHit_SQ'");
	CreateSoundCue("P_Rifle_QSkill_LaserFire", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_LaserFire_SQ.TFT_Player_LaserFire_SQ'");
	// FootStep
	CreateSoundCue("P_FootStep", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_FootStep_SQ.TFT_Player_FootStep_SQ'");
	// Dash
	CreateSoundCue("P_Dash", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_Dash_SQ.TFT_Player_Dash_SQ'");

	// Grux
	CreateSoundCue("Grux_Swing", "/Script/Engine.SoundCue'/Game/Blueprints/Monster/BossMonster_Grux/FX/SFX/Cue/TFT_Grux_Swing_SQ.TFT_Grux_Swing_SQ'");
	
	// Rampage
	CreateSoundCue("Rampage_normalPunch", "/Script/Engine.SoundCue'/Game/Blueprints/Monster/Sound/Punch.Punch'");

	// BossBJ
	CreateSoundCue("Boss_Attack", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_Sword_Swing_SQ.TFT_Player_Sword_Swing_SQ'");
	CreateSoundCue("Boss_FootStep", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_FootStep_SQ.TFT_Player_FootStep_SQ'");
	CreateSoundCue("Boss_Dash", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/Cue/TFT_Player_Dash_SQ.TFT_Player_Dash_SQ'");
	CreateSoundCue("Boss_Elec", "/Script/Engine.SoundCue'/Game/Electric_Shock_SFX_SK/CUE/Boss_Elec_sound.Boss_Elec_sound'");
	//CreateSoundCue("", "");
}

void ATFT_SoundManager::BeginPlay()
{
	Super::BeginPlay();
}

void ATFT_SoundManager::CreateSoundCue(FString keyName, FString path)
{
	ConstructorHelpers::FObjectFinder<USoundCue> sound(*path);
	if (sound.Succeeded())
	{
		USoundCue* soundCue = sound.Object;
		_table.Add(keyName, soundCue);
	}
}

void ATFT_SoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_SoundManager::PlaySound(FString keyName, FVector playLocation, FRotator playRotator)
{
	if (_table.Contains(keyName) == false) return;

	_table[keyName]->VolumeMultiplier = 1.0f;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), _table[keyName], playLocation, playRotator);
}

void ATFT_SoundManager::FadeOutSound(FString keyName, float intensity)
{
	if (_table.Contains(keyName) == false) return;

	if (_table[keyName]->VolumeMultiplier <= 0.0f) return;

	_table[keyName]->VolumeMultiplier -= intensity;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, keyName, intensity]()
		{
			this->FadeOutSound(keyName, intensity);
		}, 0.05f, false);

}
