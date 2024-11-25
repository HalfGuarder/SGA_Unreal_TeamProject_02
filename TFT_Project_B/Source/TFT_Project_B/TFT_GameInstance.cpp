// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_GameInstance.h"

#include "TFT_UIManager.h"
#include "TFT_EffectManager.h"
#include "TFT_SoundManager.h"

UTFT_GameInstance::UTFT_GameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> dataTable
	(TEXT("/Script/Engine.DataTable'/Game/Data/TFT_StatDataTable.TFT_StatDataTable'"));

	if (dataTable.Succeeded())
	{
		_statTable = dataTable.Object;
		UE_LOG(LogTemp, Error, TEXT("StatTable Load Complete"));
	}
}

void UTFT_GameInstance::Init()
{
	Super::Init();

	SpawnManager();
}

FTFT_StatData* UTFT_GameInstance::GetStatDataByLevel(int32 level)
{
	auto statData = _statTable->FindRow<FTFT_StatData>(*FString::FromInt(level), TEXT(""));
	return statData;
}

ATFT_UIManager* UTFT_GameInstance::GetUIManager()
{
	if (!_uiManager)
		SpawnManager();
		
	return _uiManager;
}

ATFT_EffectManager* UTFT_GameInstance::GetEffectManager()
{
	if(!_effectManager)
		SpawnManager();

	return _effectManager;
}

ATFT_SoundManager* UTFT_GameInstance::GetSoundManager()
{
	if(!_soundManager)
		SpawnManager();

	return _soundManager;
}

void UTFT_GameInstance::SpawnManager()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UTFT_GameInstance::SpawnManager - World is invalid."));
		return;
	}

	FActorSpawnParameters Params;
	if (!_uiManager || !_uiManager->IsValidLowLevel())
	{
		Params.Name = TEXT("UIManager");
		_uiManager = World->SpawnActor<ATFT_UIManager>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
	if (!_effectManager || !_effectManager->IsValidLowLevel())
	{
		Params.Name = TEXT("EffectManager");
		_effectManager = World->SpawnActor<ATFT_EffectManager>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
	if (!_soundManager || !_soundManager->IsValidLowLevel())
	{
		Params.Name = TEXT("SoundManager");
		_soundManager = World->SpawnActor<ATFT_SoundManager>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
}
