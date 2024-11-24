// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_Menu.generated.h"


DECLARE_MULTICAST_DELEGATE(MenuEvent);
/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_Menu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;


	UFUNCTION()
	void ContinueEvent();
	UFUNCTION()
	void OpsionEvent();
	UFUNCTION()
	void StartPageEvent();
	UFUNCTION()
	void ExitEvent();

	MenuEvent _MenuContinueEvent;
	MenuEvent _MenuStartPageEvent;
private:

	
};
