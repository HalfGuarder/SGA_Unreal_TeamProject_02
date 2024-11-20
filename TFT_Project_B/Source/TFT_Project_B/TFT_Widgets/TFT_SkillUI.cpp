// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_SkillUI.h"

#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

bool UTFT_SkillUI::Initialize()
{
	bool result = Super::Initialize();

	//
	auto array = SkillUI_CP->GetAllChildren();

	for (auto child : array)
	{
		auto slot = Cast<UTFT_Skill_piece>(child);

		if (slot)
		{
			_skillSlots.Add(slot);
		}
	}


	for (int32 i = 0; i < _skillSlots.Num(); i++)
	{
		if (i == 0) _skillSlots[i]->SetSlotNumText("Q");
		if (i == 1) _skillSlots[i]->SetSlotNumText("E");
	}//


	return result;
}

void UTFT_SkillUI::SetSkillSlots(WEAPON_TYPE itemtype, int32 slotNum, float CoolDownTime)
{
	_skillSlots[slotNum]->SetCDT(CoolDownTime);
	_skillSlots[slotNum]->SetSlotImg(itemtype, slotNum);


}

void UTFT_SkillUI::ResetSkillSlot()
{
	for (int32 i = 0; i < _skillSlots.Num(); i++)
	{
		_skillSlots[i]->ResetSlotImg();
	}
}

void UTFT_SkillUI::RunCDT(int32 slotNum)
{
	if (_skillSlots[slotNum]->bCoolDownOn == false)
	{
		_skillSlots[slotNum]->bCoolDownOn = true;
	}


}
