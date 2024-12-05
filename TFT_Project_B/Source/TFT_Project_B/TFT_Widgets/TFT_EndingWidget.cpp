// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_EndingWidget.h"

#include "Components/Button.h"

bool UTFT_EndingWidget::Initialize()
{
	bool abc = Super::Initialize();

	
	if (UButton* StartPageButton = Cast<UButton>(GetWidgetFromName(TEXT("StartPage_Button"))))
	{
		StartPageButton->OnClicked.AddDynamic(this, &UTFT_EndingWidget::StartPageEvent);
	}

	return abc;
}

void UTFT_EndingWidget::StartPageEvent()
{
	_StartPageDelegate.Broadcast();
}
