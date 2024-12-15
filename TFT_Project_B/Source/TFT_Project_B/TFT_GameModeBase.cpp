// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_GameModeBase.h"
#include "TFT_GameInstance.h"
#include "TFT_UIManager.h"
#include "TFT_MonsterSpawnManager.h"
#include "TFT_Widgets/TFT_GameStartWidget.h"
#include "Kismet/GameplayStatics.h"

#include "TFT_Player.h"

ATFT_GameModeBase::ATFT_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<ATFT_Player> player
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Characters/Player/TFT_Player_BP.TFT_Player_BP_C'"));
	if (player.Succeeded())
	{
		_player = player.Class;
	}

	if (UClass* StartWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_GameStartWidget_BP.TFT_GameStartWidget_BP_C'")))
	{
		GameStartInstance = CreateWidget<UTFT_GameStartWidget>(GetWorld(), StartWidgetClass);
	}
}

void ATFT_GameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GameStartInstance->IsValidLowLevel())
	{
		GameStartInstance->_StartEvent.AddDynamic(this, &ATFT_GameModeBase::GameStart);
	}

	GAMEINSTANCE->_reStartDelegate.AddUObject(this, &ATFT_GameModeBase::ReStart);
}

void ATFT_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (GAMEINSTANCE->_reStartTrg == false)
	{
		if (GameStartInstance)
		{
			GameStartInstance->AddToViewport(9999);
		}
		MouseUnLock();
	}
	else
	{
		GameStart();
	}
}

void ATFT_GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_playTime += DeltaTime;

	if (_playTime >= 90.0f && !bGameEnd && SPAWNMANAGER->IsAllCleared())
	{
		UIMANAGER->EndingUIOn();

		bGameEnd = true;
	}
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

	GAMEINSTANCE->_reStartTrg = false;

	MouseLock();
}

void ATFT_GameModeBase::ReStart()
{
	if (GameStartInstance)
	{
		GameStartInstance->RemoveFromViewport();
	}

	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = World->GetMapName();
		CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix); // ���� �̸����� Prefix ����
		UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));  // ���� ������ ���� ����
	}

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
		PC->SetInputMode(InputMode);  // �Է� ��带 �������� ��ȯ
		PC->bShowMouseCursor = false;  // ���콺 Ŀ���� ����
	}
}