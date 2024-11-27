// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_DeathWidget.h"

#include "Components/Button.h"

bool UTFT_DeathWidget::Initialize()
{
	bool abc = Super::Initialize();

	if (UButton* ReStartButton = Cast<UButton>(GetWidgetFromName(TEXT("ReStart_Button"))))
	{
		ReStartButton->OnClicked.AddDynamic(this, &UTFT_DeathWidget::ReStartEvent);
	}
	if (UButton* StartPageButton = Cast<UButton>(GetWidgetFromName(TEXT("StartPage_Button"))))
	{
		StartPageButton->OnClicked.AddDynamic(this, &UTFT_DeathWidget::StartPageEvent);
	}

	return abc;
}

void UTFT_DeathWidget::ReStartEvent()
{
	_ReStartDelegate.Broadcast();

}

void UTFT_DeathWidget::StartPageEvent()
{
	_StartPageDelegate.Broadcast();

}

