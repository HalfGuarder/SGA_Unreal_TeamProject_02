// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_Skill_piece.h"
#include "TFT_SkillUI.generated.h"

UCLASS()
class TFT_PROJECT_B_API UTFT_SkillUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UTFT_Skill_piece* GetSkillSlot(int32 slotNum) { return _skillSlots[slotNum]; }

	void SetSkillSlot(WEAPON_TYPE itemtype, int32 slum, float CoolDownTime);
	void HiddenSkillSlot();
	void VisbleSkillSlot(WEAPON_TYPE itemtype);

	void RunCDT(int32 slotNum);
	
	void ThisBulletText(int32 cur, int32 all);
	void SetClosedWeaponStackText();
	void SwapWeaponStackText();
public:
	TArray<class UTexture2D*> _skillImgs;

	TArray<class UTFT_Skill_piece*> _skillSlots;

	UPROPERTY(meta = (BindWidget))

	class UCanvasPanel* SkillUI_CP;


	UPROPERTY(meta = (BindWidget))
	class UImage* Weapon_Img;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BulletStack_Text;

	FText _InfinitySymbol = FText::FromString(TEXT("\u221E"));
	FString _BuletString;
private:
	UTexture2D* _SwordImg;
	UTexture2D* _GunImg;
	bool _MyWeapon = false;
};
