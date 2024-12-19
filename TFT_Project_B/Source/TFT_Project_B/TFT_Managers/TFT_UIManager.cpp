// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_UIManager.h"

#include "TFT_InvenWidget.h"
#include "TFT_EquipmentWidget.h"
#include "TFT_SkillUI.h"
#include "TFT_Menu.h"
#include "TFT_DeathWidget.h"
#include "TFT_EndingWidget.h"
#include "TFT_RandomBoxWidget.h"
#include "TFT_GameStartWidget.h"
#include "Kismet/GameplayStatics.h"

#include "TFT_GameInstance.h"

ATFT_UIManager::ATFT_UIManager()
{
 	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> startWidget
	(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_GameStartWidget_BP.TFT_GameStartWidget_BP_C'"));
	if (startWidget.Succeeded())
	{
		_GameStartInstance = CreateWidget<UTFT_GameStartWidget>(GetWorld(), startWidget.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> crossHair
	(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_CrossHair_BP.TFT_CrossHair_BP_C'"));
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

	static ConstructorHelpers::FClassFinder<UUserWidget> DeathWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_DeathWidget_BP.TFT_DeathWidget_BP_C'"));
	if (skillUI.Succeeded())
	{
		_DeathWidget = CreateWidget<UTFT_DeathWidget>(GetWorld(), DeathWidget.Class);
	}


	static ConstructorHelpers::FClassFinder<UUserWidget> tutorial(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_Tutorial_KeyInfo_BP.TFT_Tutorial_KeyInfo_BP_C'"));
	if (tutorial.Succeeded())
	{
		_tutorial = CreateWidget<UUserWidget>(GetWorld(), tutorial.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ending(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_EndingWidget_BP.TFT_EndingWidget_BP_C'"));
	if (tutorial.Succeeded())
	{
		_EndingWidget = CreateWidget<UTFT_EndingWidget>(GetWorld(), ending.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RandomBox(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/TFT_RandomBoxWidget_BP.TFT_RandomBoxWidget_BP_C'"));
	if (tutorial.Succeeded())
	{
		_RandomBoxWidget = CreateWidget<UTFT_RandomBoxWidget>(GetWorld(), RandomBox.Class);
	}

	_widgets.Add(_crossHair);
	_widgets.Add(_invenWidget);
	_widgets.Add(_EquipmentWidget);
	_widgets.Add(_SkillWidget);
	_widgets.Add(_MenuWidget);
	_widgets.Add(_DeathWidget);
	_widgets.Add(_tutorial);
	_widgets.Add(_EndingWidget);
	_widgets.Add(_RandomBoxWidget);
	_widgets.Add(_GameStartInstance);
}

void ATFT_UIManager::BeginPlay()
{
	Super::BeginPlay();

	
	//if (_tutorial)
	//{
	//	_tutorial->AddToViewport();

	//	
	//	FTimerHandle TutorialTimerHandle;
	//	GetWorld()->GetTimerManager().SetTimer(TutorialTimerHandle, FTimerDelegate::CreateLambda([this]()
	//		{
	//			if (_tutorial)
	//			{
	//				_tutorial->SetVisibility(ESlateVisibility::Hidden);
	//			}
	//		}), 10.0f, false); 
	//}

	
	OnoffWidget(UIType::Menu);
	OpenWidget(UIType::SkillUI);

	_invenOpenEvent.AddUObject(this, &ATFT_UIManager::OpenInvenUIA);
	_invenWidget->_CloseInvenBtn.AddUObject(this, &ATFT_UIManager::CloseInvenBtn);
	_EquipmentOpenEvent.AddUObject(this, &ATFT_UIManager::OnOffEquipmentUIA);
	_EquipmentWidget->_CloseEquipmentBtn.AddUObject(this, &ATFT_UIManager::CloseEquipmentUIA);
	_OpenCrossHairEvent.AddUObject(this, &ATFT_UIManager::OpenClossHairUI);
	_CloseCrossHairEvent.AddUObject(this, &ATFT_UIManager::CloseClossHairUI);

	_MenuOpenEvent.AddUObject(this, &ATFT_UIManager::OnOffPlayMenu);
	_MenuWidget->_MenuContinueEvent.AddUObject(this, &ATFT_UIManager::OnOffPlayMenu);
	_MenuWidget->_MenuStartPageEvent.AddUObject(this, &ATFT_UIManager::ResetLevel);

	_DeathWidget->_startPageDelegate.AddUObject(this, &ATFT_UIManager::DeathResetLevel);
	_DeathWidget->_ReStartDelegate.AddUObject(this, &ATFT_UIManager::ReStart);

	_EndingWidget->_startPageDelegate.AddUObject(this, &ATFT_UIManager::ResetLevel);

	_RandomBoxOpenEvent.AddUObject(this, &ATFT_UIManager::RandomBoxUIA);
	_RandomBoxWidget->_SelectEvent.AddUObject(this, &ATFT_UIManager::RandomBoxUIA);
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

void ATFT_UIManager::OpenClossHairUI()
{
	/*if (_UICrossHair == false)
	{
		_UICrossHair = true;
		OpenWidget(UIType::CrossHair);
	}
	else if (_UICrossHair == true)
	{
		_UICrossHair = false;
		CloseWidget(UIType::CrossHair);
	}*/
	OpenWidget(UIType::CrossHair);
}

void ATFT_UIManager::CloseClossHairUI()
{
	CloseWidget(UIType::CrossHair);
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

void ATFT_UIManager::DeathUIA()
{
	if (_UIDeath == false)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.3f);
		_UIDeath = true;

		OpenWidget(UIType::DeathUI);
		MouseUnLock(UIType::DeathUI);
	}
	else if (_UIDeath == true)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
		_UIDeath = false;

		CloseWidget(UIType::DeathUI);
		MouseLock(UIType::DeathUI);
	}
}

void ATFT_UIManager::EndingUIOn()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);

	OpenWidget(UIType::EndingUI);
	MouseUnLock(UIType::EndingUI);
}

void ATFT_UIManager::RandomBoxUIA()
{
	if (_UIRandom == false)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);
		_UIRandom = true;

		_RandomBoxWidget->RandomBoxSetting(1);
		_RandomBoxWidget->RandomBoxSetting(2);

		OpenWidget(UIType::RandomBoxUI);
		MouseUnLock(UIType::RandomBoxUI);
	}
	else if (_UIRandom == true)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
		_UIRandom = false;

		CloseWidget(UIType::RandomBoxUI);
		MouseLock(UIType::RandomBoxUI);
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

void ATFT_UIManager::ResetLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = World->GetMapName();
		CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix); // 레벨 이름에서 Prefix 제거
		UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
	}
}

void ATFT_UIManager::DeathResetLevel()
{
	DeathUIA();

	ResetLevel();
}

void ATFT_UIManager::ReStart()
{
	DeathUIA();

	GAMEINSTANCE->ReStart();
}

