// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_GameStartWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void UTFT_GameStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* StartButton = Cast<UButton>(GetWidgetFromName(TEXT("GameStart_Button"))))
	{
		StartButton->OnClicked.AddDynamic(this, &UTFT_GameStartWidget::StartEvent);
	}

	if (UButton* ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Exit_Button"))))
	{
		ExitButton->OnClicked.AddDynamic(this, &UTFT_GameStartWidget::ExitEvent);
	}

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

	UE_LOG(LogTemp, Error, TEXT("Game End")); // Test용 로그

	// QuitGame 함수 호출
	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}

void UTFT_GameStartWidget::HideUI()
{
	this->RemoveFromParent();
}
