// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_EndingWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(EndingEvent)
/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_EndingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	

	UFUNCTION()
	void StartPageEvent();

	EndingEvent _startPageDelegate;
};
