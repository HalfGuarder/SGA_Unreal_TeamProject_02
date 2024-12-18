// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_LoadingScreen.generated.h"

UCLASS()
class TFT_PROJECT_B_API UTFT_LoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	UFUNCTION()
	void StartEvent();
	UFUNCTION()
	void ExitEvent();

protected:
	void HideUI();
};