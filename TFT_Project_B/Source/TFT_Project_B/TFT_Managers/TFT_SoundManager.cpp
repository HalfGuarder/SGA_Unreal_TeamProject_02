// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_SoundManager.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ATFT_SoundManager::ATFT_SoundManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateSoundCue("Explosion", "/Script/Engine.SoundCue'/Game/Blueprints/SFX/Rampage_Sound/explosion.explosion'");

	// Player
	CreateSoundCue("P_Fire", "/Script/Engine.SoundCue'/Game/Blueprints/Characters/Player/FX/SFX/SoundCue/TFT_Player_Rifle_Fire_SQ.TFT_Player_Rifle_Fire_SQ'");
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

void ATFT_SoundManager::Play(FString keyName, FVector playLocation, FRotator playRotator)
{
	if (_table.Contains(keyName) == false) return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), _table[keyName], playLocation, playRotator);
}

