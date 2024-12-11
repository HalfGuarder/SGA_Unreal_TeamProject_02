// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_ExpBar.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_ExpBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetExpBarValue(float ratio);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_ExpBar;
};
