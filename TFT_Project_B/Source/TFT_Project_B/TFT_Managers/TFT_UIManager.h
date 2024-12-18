// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_UIManager.generated.h"

class UUserWIdget;
class UTFT_InvenWidget;
class UTFT_EquipmentWidget;

DECLARE_MULTICAST_DELEGATE(UIOpenEvent);

UENUM()
enum class UIType : int32
{
	CrossHair,
	Inventory,
	SkillUI,
	EquipmentUI,
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

	UTFT_InvenWidget* GetInvenUI() { return _invenWidget; }
	UTFT_EquipmentWidget* GetEquipmentUI() { return _EquipmentWidget; }

	UFUNCTION(BlueprintCallable)
	TArray<UUserWidget*> GetWidgets() { return _widgets; }

	void MouseUnLock(UIType type);
	void MouseLock(UIType type);

	UIOpenEvent _invenOpenEvent;
	UIOpenEvent _EquipmentOpenEvent;
	UIOpenEvent _EquipmentCloseResetEvent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UUserWidget*> _widgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_InvenWidget* _invenWidget;

	bool _UIInvenarea = false;
	bool _UIEquipmentarea = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	UTFT_EquipmentWidget* _EquipmentWidget;
};
