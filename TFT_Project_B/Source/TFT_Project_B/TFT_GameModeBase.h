// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFT_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATFT_GameModeBase();

	void Init();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void GameStart();

	void MouseUnLock();
	void MouseLock();
private:
	UPROPERTY()
	class UTFT_GameStartWidget* GameStartInstance;

	TSubclassOf<APawn> _player;
};
