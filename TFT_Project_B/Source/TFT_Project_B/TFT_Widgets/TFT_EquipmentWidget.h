// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_EquipmentWidget.generated.h"

class UButton;
class ATFT_Item;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(ItemChangeEvent, ATFT_Item* item)

DECLARE_MULTICAST_DELEGATE(CloseEquipmentBtn);

UCLASS()
class TFT_PROJECT_B_API UTFT_EquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	UFUNCTION()
	void EquipmentBtnClose();
	UFUNCTION()
	void Set_TakeOff();

	UFUNCTION()
	void SelectSlotItem(int32 index);

	UFUNCTION()
	void SetTo_Head();
	UFUNCTION()
	void SetTo_Chest();
	UFUNCTION()
	void SetTo_Leg();
	UFUNCTION()
	void SetTo_Foot();
	UFUNCTION()
	void SetTo_L_Weapon();
	UFUNCTION()
	void SetTo_R_Weapon();

	void Set_Equipment(ATFT_Item* item);
	void Un_Equipment(int32 index);
	void Changes(ATFT_Item* item, int32 index);
	UFUNCTION()
	void SetItemChoice(UTexture2D* texture, int32 slotIndex);
	void ResetChoice();


	CloseEquipmentBtn _CloseEquipmentBtn;
	ItemChangeEvent _ItemChangeEvent;
	ItemChangeEvent _ItemChangeEvent_stat;

private:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* TFT_Equipment;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UButton*> _slotBtns;

	UPROPERTY(meta = (BindWidget))
	UButton* TFT_headSolt;
	UPROPERTY(meta = (BindWidget))
	UButton* TFT_ChestSlot;
	UPROPERTY(meta = (BindWidget))
	UButton* TFT_LegSlot;
	UPROPERTY(meta = (BindWidget))
	UButton* TFT_FootSlot;
	UPROPERTY(meta = (BindWidget))
	UButton* TFT_R_WeaponSlot;
	UPROPERTY(meta = (BindWidget))
	UButton* TFT_L_WeaponSlot;


	UPROPERTY(meta = (BindWidget))
	UButton* TakeOff_Button;
	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Button;

	UTexture2D* _emptySlot;
	UTexture2D* _choiceSlot;

	UPROPERTY(meta = (BindWidget))
	UImage* Use_Head;
	UPROPERTY(meta = (BindWidget))
	UImage* Use_Chest;
	UPROPERTY(meta = (BindWidget))
	UImage* Use_Leg;
	UPROPERTY(meta = (BindWidget))
	UImage* Use_Foot;
	UPROPERTY(meta = (BindWidget))
	UImage* Use_RWeapon;
	UPROPERTY(meta = (BindWidget))
	UImage* Use_LWeapon;

private:
	TArray<ATFT_Item*> _EquipmentItem;

	ATFT_Item* this_Item;
	int32 this_Index = -1;
	bool choiceCheck = false;
};
