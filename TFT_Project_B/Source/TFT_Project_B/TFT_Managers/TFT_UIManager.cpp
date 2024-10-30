// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_UIManager.h"

#include "TFT_InvenWidget.h"
#include "TFT_EquipmentWidget.h"
#include "Kismet/GameplayStatics.h"

ATFT_UIManager::ATFT_UIManager()
{
 	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> invenWidget
	(TEXT(""));
	if (invenWidget.Succeeded())
	{
		_invenWidget = CreateWidget<UTFT_InvenWidget>(GetWorld(), invenWidget.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> EquipmentWidget
	(TEXT(""));
	if (EquipmentWidget.Succeeded())
	{
		_EquipmentWidget = CreateWidget<UTFT_EquipmentWidget>(GetWorld(), EquipmentWidget.Class);
	}

	_widgets.Add(_invenWidget);
	_widgets.Add(_EquipmentWidget);
}

void ATFT_UIManager::BeginPlay()
{
	Super::BeginPlay();
	
	OpenWidget(UIType::Inventory);
	CloseWidget(UIType::Inventory);

	// OpenWidget(UIType::CrossHair);
	OpenWidget(UIType::SkillUI);

	OpenWidget(UIType::EquipmentUI);
	CloseWidget(UIType::EquipmentUI);

	// OpenWidget(UIType::PartyHPUI);

	_invenOpenEvent.AddUObject(this, &ATFT_UIManager::OpenInvenUIA);
	_invenWidget->_CloseInvenBtn.AddUObject(this, &ATFT_UIManager::CloseInvenBtn);
	_EquipmentOpenEvent.AddUObject(this, &ATFT_UIManager::OnOffEquipmentUIA);
	_EquipmentWidget->_CloseEquipmentBtn.AddUObject(this, &ATFT_UIManager::CloseEquipmentUIA);
}

void ATFT_UIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_UIManager::OpenWidget(UIType type)
{
	int32 typeNum = (int32)type;
	if (_widgets.Num() <= typeNum) return;

	_widgets[typeNum]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_widgets[typeNum]->AddToViewport(typeNum);
}

void ATFT_UIManager::CloseWidget(UIType type)
{
	int32 typeNum = (int32)type;
	if (_widgets.Num() <= typeNum)
		return;

	_widgets[typeNum]->SetVisibility(ESlateVisibility::Hidden);
	_widgets[typeNum]->RemoveFromViewport();
}

void ATFT_UIManager::CloseAll()
{
	for (auto widget : _widgets)
	{
		widget->SetVisibility(ESlateVisibility::Hidden);
		widget->RemoveFromViewport();
	}
}

void ATFT_UIManager::OpenInvenUIA()
{
	if (_UIInvenarea == false)
	{
		UE_LOG(LogTemp, Log, TEXT("ainven : open"));
		_UIInvenarea = true;
		OpenWidget(UIType::Inventory);
		MouseUnLock(UIType::Inventory);
	}
	else if (_UIInvenarea == true)
	{
		UE_LOG(LogTemp, Log, TEXT("inven : close"));
		_UIInvenarea = false;
		CloseWidget(UIType::Inventory);
		MouseLock(UIType::Inventory);
	}
}

void ATFT_UIManager::CloseInvenBtn()
{
	UE_LOG(LogTemp, Log, TEXT("inven : close"));
	_UIInvenarea = false;
	CloseWidget(UIType::Inventory);
	MouseLock(UIType::Inventory);
}

void ATFT_UIManager::OnOffEquipmentUIA()
{
	if (_UIEquipmentarea == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Equipment : open"));
		_UIEquipmentarea = true;
		OpenWidget(UIType::EquipmentUI);
		MouseUnLock(UIType::EquipmentUI);
	}
	else if (_UIEquipmentarea == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Equipment : close"));
		_UIEquipmentarea = false;
		_EquipmentCloseResetEvent.Broadcast();
		CloseWidget(UIType::EquipmentUI);
		MouseLock(UIType::EquipmentUI);
	}
}

void ATFT_UIManager::CloseEquipmentUIA()
{
	UE_LOG(LogTemp, Log, TEXT("Equipment : close"));
	_UIEquipmentarea = false;
	_EquipmentCloseResetEvent.Broadcast();
	CloseWidget(UIType::EquipmentUI);
	MouseLock(UIType::EquipmentUI);
}

void ATFT_UIManager::MouseUnLock(UIType type)
{
	int32 typeNum = (int32)type;
	if (_widgets.Num() <= typeNum)
		return;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(_widgets[typeNum]->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void ATFT_UIManager::MouseLock(UIType type)
{
	int32 typeNum = (int32)type;
	if (_widgets.Num() <= typeNum)
		return;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}

