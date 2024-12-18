// Fill out your copyright notice in the Description page of Project Settings.

#include "TFT_Widgets/TFT_GameStartWidget.h"
#include "TFT_Widgets/TFT_LoadingScreen.h"
#include "TFT_UIManager.h"
#include "Kismet/GameplayStatics.h"
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

	ShowLoadingScreen();
}

void UTFT_GameStartWidget::ExitEvent()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
    
	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}

void UTFT_GameStartWidget::ShowLoadingScreen()
{
    UWorld* World = GetWorld();
    if (!World) return;

    ATFT_UIManager* UIManager = Cast<ATFT_UIManager>(UGameplayStatics::GetActorOfClass(World, ATFT_UIManager::StaticClass()));
    if (UIManager)
    {
        UIManager->CloseAll();
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->SetIgnoreMoveInput(true);
        PlayerController->SetIgnoreLookInput(true);
    }

    const FString WidgetPath = TEXT("/Game/Blueprints/Widget/TFT_Loading_Screen.TFT_Loading_Screen_C");
    UClass* WidgetClass = LoadObject<UClass>(nullptr, *WidgetPath);

    if (WidgetClass)
    {
        UUserWidget* LoadingScreen = CreateWidget<UUserWidget>(World, WidgetClass);
        if (LoadingScreen)
        {
            LoadingScreen->AddToViewport();

            FTimerHandle TimerHandle;
            World->GetTimerManager().SetTimer(
                TimerHandle,
                FTimerDelegate::CreateLambda([LoadingScreen, UIManager, PlayerController]()
                    {
                        if (LoadingScreen->IsInViewport())
                        {
                            LoadingScreen->RemoveFromParent();
                        }

                        if (UIManager)
                        {
                            UIManager->OpenWidget(UIType::SkillUI);
                        }

                        if (PlayerController)
                        {
                            PlayerController->SetIgnoreMoveInput(false);
                            PlayerController->SetIgnoreLookInput(false);
                        }
                    }),
                3.0f,
                false
            );
        }
    }
}

void UTFT_GameStartWidget::HideUI()
{
	this->RemoveFromParent();
}
