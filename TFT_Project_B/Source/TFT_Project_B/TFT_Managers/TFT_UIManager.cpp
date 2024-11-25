// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_UIManager.h"

#include "TFT_InvenWidget.h"
#include "TFT_EquipmentWidget.h"
#include "TFT_SkillUI.h"
#include "TFT_Menu.h"
#include "Kismet/GameplayStatics.h"

ATFT_UIManager::ATFT_UIManager()
{
 	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> crossHair(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_CrossHair_BP.TFT_CrossHair_BP_C'"));
	if (crossHair.Succeeded())
	{
		_crossHair = CreateWidget<UUserWidget>(GetWorld(), crossHair.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> invenWidget
	(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_InvenWidget_BP.TFT_InvenWidget_BP_C'"));
	if (invenWidget.Succeeded())
	{
		_invenWidget = CreateWidget<UTFT_InvenWidget>(GetWorld(), invenWidget.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> EquipmentWidget
	(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/UTFT_EquipmentWidget_BP.UTFT_EquipmentWidget_BP_C'"));
	if (EquipmentWidget.Succeeded())
	{
		_EquipmentWidget = CreateWidget<UTFT_EquipmentWidget>(GetWorld(), EquipmentWidget.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> skillUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_SkillUI_BP.TFT_SkillUI_BP_C'"));
	if (skillUI.Succeeded())
	{
		_SkillWidget = CreateWidget<UTFT_SkillUI>(GetWorld(), skillUI.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_Menu_BP.TFT_Menu_BP_C'"));
	if (skillUI.Succeeded())
	{
		_MenuWidget = CreateWidget<UTFT_Menu>(GetWorld(), MenuWidget.Class);
	}

	_widgets.Add(_crossHair);
	_widgets.Add(_invenWidget);
	_widgets.Add(_EquipmentWidget);
	_widgets.Add(_SkillWidget);
	_widgets.Add(_MenuWidget);
}

void ATFT_UIManager::BeginPlay()
{
	Super::BeginPlay();
	
	//OpenWidget(UIType::Inventory);
	//CloseWidget(UIType::Inventory);
	//OpenWidget(UIType::EquipmentUI);
	//CloseWidget(UIType::EquipmentUI);

	OnoffWidget(UIType::Menu);

	OpenWidget(UIType::SkillUI);


	_invenOpenEvent.AddUObject(this, &ATFT_UIManager::OpenInvenUIA);
	_invenWidget->_CloseInvenBtn.AddUObject(this, &ATFT_UIManager::CloseInvenBtn);
	_EquipmentOpenEvent.AddUObject(this, &ATFT_UIManager::OnOffEquipmentUIA);
	_EquipmentWidget->_CloseEquipmentBtn.AddUObject(this, &ATFT_UIManager::CloseEquipmentUIA);
	_WeaponZoomEvent.AddUObject(this, &ATFT_UIManager::WeaponCrossHairUIA);

	_MenuOpenEvent.AddUObject(this, &ATFT_UIManager::OnOffPlayMenu);
	_MenuWidget->_MenuContinueEvent.AddUObject(this, &ATFT_UIManager::OnOffPlayMenu);
	_MenuWidget->_MenuStartPageEvent.AddUObject(this, &ATFT_UIManager::RsetLevel);
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

void ATFT_UIManager::OnoffWidget(UIType type)
{
	OpenWidget(type);

	CloseWidget(type);
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

void ATFT_UIManager::WeaponCrossHairUIA()
{
	if (_UICrossHair == false)
	{
		_UICrossHair = true;
		OpenWidget(UIType::CrossHair);
	}
	else if (_UICrossHair == true)
	{
		_UICrossHair = false;
		CloseWidget(UIType::CrossHair);
	}
}

void ATFT_UIManager::OnOffPlayMenu()
{
	if (_UIPlayMenu == false)
	{
		// 시간 정지
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);
		_UIPlayMenu = true;

		OpenWidget(UIType::Menu);
		MouseUnLock(UIType::Menu);
	}
	else if (_UIPlayMenu == true)
	{
		// 시간 재개
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
		_UIPlayMenu = false;

		CloseWidget(UIType::Menu);
		MouseLock(UIType::Menu);
	}
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

void ATFT_UIManager::RsetLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = World->GetMapName();
		CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix); // 레벨 이름에서 Prefix 제거
		UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
	}
}

