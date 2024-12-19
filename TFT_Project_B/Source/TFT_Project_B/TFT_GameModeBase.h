// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFT_GameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(GameModeEvent);
/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATFT_GameModeBase();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void GameStart();

	void ReStart();

	void MouseUnLock();

	void MouseLock();

	GameModeEvent _reEvent;

	float GetPlayTime() { return _playTime; }

private:
	/*UPROPERTY()
	class UTFT_GameStartWidget* GameStartInstance;*/

	TSubclassOf<APawn> _player;

	bool _restart = false;

	float _playTime = 0.0f;
	bool bGameEnd = false;
};
