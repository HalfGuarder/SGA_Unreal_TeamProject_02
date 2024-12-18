// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_RandomBoxWidget.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(RandomBoxChoice, int32 type, int32 Value)
DECLARE_MULTICAST_DELEGATE(RandomBoxEvent)
/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_RandomBoxWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	void RandomBoxSetting(int32 num);

	UFUNCTION()
	void SelectLeftEvent();

	UFUNCTION()
	void SelectRightEvent();

	UPROPERTY(meta = (BindWidget))
	class UImage* RandomImg_Left;

	UPROPERTY(meta = (BindWidget))
	UImage* RandomImg_Right;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeftText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RightText;


	RandomBoxEvent _SelectEvent;
	RandomBoxChoice _LeftEvent;
	RandomBoxChoice _RightEvent;
private:
	class UTexture2D* _InvisibleImg;

	UTexture2D* _PlusHpImg;
	UTexture2D* _PlusBarrierImg;
	UTexture2D* _AddAmmoImg;

	UTexture2D* _HPrecrveryImg;
	UTexture2D* _SupplyImg;
	UTexture2D* _BarrierRecrveryImg;

	//FText _maxHPstring = FText::FromString(TEXT("최대 체력"));
	//FText _maxBarrierstring = FText::FromString(TEXT("최대 보호막"));
	//FText _maxAmmostring = FText::FromString(TEXT("최대 장전량"));

	int32 _LeftValue;
	int32 _LeftPlusValue;
	int32 _RightValue;
	int32 _RightPlusValue;

	int32 randCheck;
};
