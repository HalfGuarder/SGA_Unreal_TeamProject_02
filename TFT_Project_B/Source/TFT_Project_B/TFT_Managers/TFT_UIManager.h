// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_UIManager.generated.h"

class UUserWIdget;
class UTFT_InvenWidget;
class UTFT_EquipmentWidget;
class UTFT_SkillUI;
class UTFT_Menu;
class UTFT_DeathWidget;

DECLARE_MULTICAST_DELEGATE(UIOpenEvent);

UENUM()
enum class UIType : int32
{
	CrossHair,
	Inventory,
	EquipmentUI,
	SkillUI,
	Menu,
	DeathUI,
	Tutorial,
};


UCLASS()
class TFT_PROJECT_B_API ATFT_UIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFT_UIManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void OpenWidget(UIType type);
	void CloseWidget(UIType type);
	void OnoffWidget(UIType type);
	void CloseAll();

	UFUNCTION()
	void OpenInvenUIA();
	UFUNCTION()
	void CloseInvenBtn();

	UFUNCTION()
	void OnOffEquipmentUIA();
	UFUNCTION()
	void CloseEquipmentUIA();

	UFUNCTION()
	void WeaponCrossHairUIA();

	UFUNCTION()
	void OnOffPlayMenu();

	UFUNCTION()
	void DeathUIA();

	UTFT_InvenWidget* GetInvenUI() { return _invenWidget; }
	UTFT_EquipmentWidget* GetEquipmentUI() { return _EquipmentWidget; }
	UTFT_SkillUI* GetSkillUI() { return _SkillWidget; }
	UTFT_Menu* GetMenuUI() { return _MenuWidget; }
	UTFT_DeathWidget* GetDeathUI() { return _DeathWidget; }

	UFUNCTION(BlueprintCallable)
	TArray<UUserWidget*> GetWidgets() { return _widgets; }

	void MouseUnLock(UIType type);
	void MouseLock(UIType type);

	UFUNCTION()
	void ResetLevel();
	UFUNCTION()
	void DeathResetLevel();
	UFUNCTION()
	void ReStart();

	UIOpenEvent _invenOpenEvent;
	UIOpenEvent _EquipmentOpenEvent;
	UIOpenEvent _EquipmentCloseResetEvent;
	UIOpenEvent _WeaponZoomEvent;
	UIOpenEvent _MenuOpenEvent;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UUserWidget*> _widgets;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UUserWidget* _crossHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_InvenWidget* _invenWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_EquipmentWidget* _EquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_SkillUI* _SkillWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_Menu* _MenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_DeathWidget* _DeathWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UUserWidget* _tutorial;

	bool _UIInvenarea = false;
	bool _UIEquipmentarea = false;
	bool _UICrossHair = false;
	bool _UIPlayMenu = false;
	bool _UIDeath = false;
};
