// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_UIManager.generated.h"

class UUserWIdget;
class UTFT_InvenWidget;
class UTFT_EquipmentWidget;
class UTFT_SkillUI;

DECLARE_MULTICAST_DELEGATE(UIOpenEvent);

UENUM()
enum class UIType : int32
{
	CrossHair,
	Inventory,
	EquipmentUI,
	SkillUI,
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

	UTFT_InvenWidget* GetInvenUI() { return _invenWidget; }
	UTFT_EquipmentWidget* GetEquipmentUI() { return _EquipmentWidget; }
	UTFT_SkillUI* GetSkillUI() { return _SkillWidget; }

	UFUNCTION(BlueprintCallable)
	TArray<UUserWidget*> GetWidgets() { return _widgets; }

	void MouseUnLock(UIType type);
	void MouseLock(UIType type);

	UIOpenEvent _invenOpenEvent;
	UIOpenEvent _EquipmentOpenEvent;
	UIOpenEvent _EquipmentCloseResetEvent;
	UIOpenEvent _WeaponZoomEvent;

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


	bool _UIInvenarea = false;
	bool _UIEquipmentarea = false;
	bool _UICrossHair = false;
};
