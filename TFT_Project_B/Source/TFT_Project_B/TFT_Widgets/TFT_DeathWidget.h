// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_DeathWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(DeathEvent)
/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_DeathWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	
	UFUNCTION()
	void ReStartEvent();
	UFUNCTION()
	void StartPageEvent();

	DeathEvent _ReStartDelegate;
	DeathEvent _StartPageDelegate;

};
