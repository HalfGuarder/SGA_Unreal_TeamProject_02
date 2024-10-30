// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_HPBarWidget.generated.h"


UCLASS()
class TFT_PROJECT_B_API UTFT_HPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHpBarValue(float ratio);
	void SetMpBarValue(float ratio);
	void SetExpBarValue(float ratio);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_MpBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_ExpBar;
};
