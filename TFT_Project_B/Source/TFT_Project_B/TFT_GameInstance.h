// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "TFT_StatComponent.h"

#include "TFT_GameInstance.generated.h"

class ATFT_UIManager;
class ATFT_EffectManager;
class ATFT_SoundManager;

#define UIMANAGER Cast<UTFT_GameInstance>(GetWorld()->GetGameInstance())->GetUIManager()
#define EFFECTMANAGER Cast<UTFT_GameInstance>(GetWorld()->GetGameInstance())->GetEffectManager()
#define SOUNDMANAGER Cast<UTFT_GameInstance>(GetWorld()->GetGameInstance())->GetSoundManager()


UCLASS()
class TFT_PROJECT_B_API UTFT_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTFT_GameInstance();

	virtual void Init() override;

	FTFT_StatData* GetStatDataByLevel(int32 level);

	UFUNCTION(BlueprintCallable)
	ATFT_UIManager* GetUIManager() { return _uiManager; }
	ATFT_EffectManager* GetEffectManager() { return _effectManager; }
	ATFT_SoundManager* GetSoundManager() { return _soundManager; }

public:
	UPROPERTY()
	class UDataTable* _statTable;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATFT_UIManager* _uiManager;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATFT_EffectManager* _effectManager;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATFT_SoundManager* _soundManager;
};
