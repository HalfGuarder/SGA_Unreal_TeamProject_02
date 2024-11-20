// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TFT_Skill_piece.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFT_SkillUI.generated.h"

class UTFT_Skill_piece;
/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_SkillUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UTFT_Skill_piece* GetSkillSlot(int32 slotNum) { return _skillSlots[slotNum]; }

	void SetSkillSlots(WEAPON_TYPE itemtype, int32 slotNum, float CoolDownTime);
	void ResetSkillSlot();


	void RunCDT(int32 slotNum);


private:

public:
	TArray<UTexture2D*> _skillImgs;

	TArray<UTFT_Skill_piece*> _skillSlots;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SkillUI_CP;

};
