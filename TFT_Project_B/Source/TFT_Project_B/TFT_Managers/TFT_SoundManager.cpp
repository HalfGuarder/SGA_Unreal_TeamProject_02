// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_SoundManager.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ATFT_SoundManager::ATFT_SoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CreateSoundCue("Sound", "/Script/Engine.SoundCue'/Game/ParagonRampage/Characters/Heroes/Rampage/Sounds/SoundCues/Rampage_Ability_Ultimate_Enemy.Rampage_Ability_Ultimate_Enemy'");
}

// Called when the game starts or when spawned
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

// Called every frame
void ATFT_SoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_SoundManager::Play(FString keyName, FVector playLocation, FRotator playRotator)
{
	if (_table.Contains(keyName) == false) return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), _table[keyName], playLocation, playRotator);
}

