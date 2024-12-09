// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_HPBarWidget.generated.h"

enum ProfileType
{
	EMPTY,
	PLAYER,
	BOSS1,
	BOSS2
};

UCLASS()
class TFT_PROJECT_B_API UTFT_HPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetHpBarValue(float ratio);
	void SetBarrierBarValue(float ratio);
	
	void SetProfileImage(ProfileType num);
	void SetHpText(int32 maxHp);
	void CurHpText(int32 curHp);

	void SetBarrierText(int32 Barrier);
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BP_HpBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BP_BarrierBar;	


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurHp_Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHp_Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurBarrierText;


	UPROPERTY(meta = (BindWidget))
	class UImage* profile;

	class UTexture2D* profile_empty;
	UTexture2D* profile_player;
	UTexture2D* profile_boss1;
	UTexture2D* profile_boss2;
	FText _nullText = FText::FromString("");

};
