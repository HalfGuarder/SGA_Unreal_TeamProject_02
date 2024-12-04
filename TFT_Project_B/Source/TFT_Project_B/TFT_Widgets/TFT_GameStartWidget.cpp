// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_GameStartWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

bool UTFT_GameStartWidget::Initialize()
{
	bool abc = Super::Initialize();

	if (UButton* StartButton = Cast<UButton>(GetWidgetFromName(TEXT("GameStart_Button"))))
	{
		StartButton->OnClicked.AddDynamic(this, &UTFT_GameStartWidget::StartEvent);
	}

	if (UButton* ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Exit_Button"))))
	{
		ExitButton->OnClicked.AddDynamic(this, &UTFT_GameStartWidget::ExitEvent);
	}



	return abc;
}

void UTFT_GameStartWidget::StartEvent()
{
	_StartEvent.Broadcast();

	HideUI();
}

void UTFT_GameStartWidget::ExitEvent()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	// QuitGame �Լ� ȣ��
	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}

void UTFT_GameStartWidget::HideUI()
{
	this->RemoveFromParent();
}
