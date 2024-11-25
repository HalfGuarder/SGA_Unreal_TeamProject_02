// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_Menu.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"



bool UTFT_Menu::Initialize()
{
	bool abc = Super::Initialize();

	if (UButton* CountinueButton = Cast<UButton>(GetWidgetFromName(TEXT("Continue_Button"))))
	{
		CountinueButton->OnClicked.AddDynamic(this, &UTFT_Menu::ContinueEvent);
	}
	if (UButton* OptionButton = Cast<UButton>(GetWidgetFromName(TEXT("Option_Button"))))
	{
		OptionButton->OnClicked.AddDynamic(this, &UTFT_Menu::OptionEvent);
	}
	if (UButton* StartPageButton = Cast<UButton>(GetWidgetFromName(TEXT("StartPage_Button"))))
	{
		StartPageButton->OnClicked.AddDynamic(this, &UTFT_Menu::StartPageEvent);
	}
	if (UButton* ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Exit_Button"))))
	{
		ExitButton->OnClicked.AddDynamic(this, &UTFT_Menu::ExitEvent);
	}
	return abc;
}

void UTFT_Menu::ContinueEvent()
{
	UE_LOG(LogTemp, Log, TEXT("Test Continue"));

	_MenuContinueEvent.Broadcast();
}

void UTFT_Menu::OptionEvent()
{
	UE_LOG(LogTemp, Log, TEXT("Test Opsion"));

	
}

void UTFT_Menu::StartPageEvent()
{
	UE_LOG(LogTemp, Log, TEXT("Test StartPage"));

	_MenuStartPageEvent.Broadcast();
}

void UTFT_Menu::ExitEvent()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	UE_LOG(LogTemp, Error, TEXT("Menu Game End")); // Test용 로그

	// QuitGame 함수 호출
	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}
