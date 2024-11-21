// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_GameModeBase.h"
#include "TFT_Widgets/TFT_GameStartWidget.h"
#include "Kismet/GameplayStatics.h"

#include "TFT_Player.h"

ATFT_GameModeBase::ATFT_GameModeBase()
{
	static ConstructorHelpers::FClassFinder<ATFT_Player> player
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Characters/Player/TFT_Player_BP.TFT_Player_BP_C'"));

	if (player.Succeeded())
	{
		_player = player.Class;
	}

    if (UClass* StartWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_GameStartWidget_BP.TFT_GameStartWidget_BP_C'")))
    {
        GameStartInstance = CreateWidget<UTFT_GameStartWidget>(GetWorld(), StartWidgetClass);
        if (GameStartInstance)
        {
            GameStartInstance->AddToViewport(9999);
        }
    }
}

void ATFT_GameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GameStartInstance->IsValidLowLevel())
	{
		GameStartInstance->_StartEvent.AddDynamic(this, &ATFT_GameModeBase::GameStart);
	}

}

void ATFT_GameModeBase::BeginPlay()
{
	Super::BeginPlay();



	MouseUnLock();
}

void ATFT_GameModeBase::GameStart()
{
	DefaultPawnClass = _player;

	FVector location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FRotator rotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();

	ATFT_Player* player = GetWorld()->SpawnActor<ATFT_Player>(_player, location, rotation);
	APawn* oldpawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	oldpawn->SetActorHiddenInGame(true);
	oldpawn->SetActorEnableCollision(false);
	GetWorld()->GetFirstPlayerController()->UnPossess();
	GetWorld()->GetFirstPlayerController()->Possess(player);
	if (oldpawn) oldpawn->Destroy();

	MouseLock();
}

void ATFT_GameModeBase::MouseUnLock()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(GameStartInstance->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void ATFT_GameModeBase::MouseLock()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);  // 입력 모드를 게임으로 전환
		PC->bShowMouseCursor = false;  // 마우스 커서를 숨김
	}
}
