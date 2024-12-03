// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_SkillUI.h"

#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


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
		if (i % 2 == 0) _skillSlots[i]->SetSlotNumText("Q");
		else _skillSlots[i]->SetSlotNumText("E");
	}



	_SwordImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Weapon_Sword.Weapon_Sword'"));
	_GunImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Weapon_gun.Weapon_gun'"));


	return result;
}

void UTFT_SkillUI::SetSkillSlot(WEAPON_TYPE itemtype, int32 slotNum, float CoolDownTime)
{
	_skillSlots[slotNum]->SetCDT(CoolDownTime);
	_skillSlots[slotNum]->SetSlotImg(itemtype, slotNum);


}

void UTFT_SkillUI::HiddenSkillSlot()
{
	for (int32 i = 0; i < _skillSlots.Num(); i++)
	{
		_skillSlots[i]->HeddenSlotImg();
	}
}

void UTFT_SkillUI::VisbleSkillSlot(WEAPON_TYPE itemtype)
{
	switch (itemtype)
	{
	case closeRange:
	{
		_skillSlots[0]->SetSlotImg(itemtype, 0);
		_skillSlots[0]->changedText = false;
		_skillSlots[1]->SetSlotImg(itemtype, 1);
		_skillSlots[1]->changedText = false;

		Weapon_Img->SetBrushFromTexture(_SwordImg);
		_MyWeapon = true;
	}
	break;
	case longLange:
	{
		_skillSlots[2]->SetSlotImg(itemtype, 2);
		_skillSlots[2]->changedText = false;
		_skillSlots[3]->SetSlotImg(itemtype, 3);
		_skillSlots[3]->changedText = false;

		Weapon_Img->SetBrushFromTexture(_GunImg);
		_MyWeapon = false;
	}
	break;
	default:
		break;
	}
}

void UTFT_SkillUI::RunCDT(int32 slotNum)
{
	if (_skillSlots[slotNum]->bCoolDownOn == false)
	{
		_skillSlots[slotNum]->bCoolDownOn = true;
	}


}

void UTFT_SkillUI::ThisBulletText(int32 cur, int32 all)
{
	if (_MyWeapon) 
	{
		BulletStack_Text->SetText(_InfinitySymbol);
		return;
	}

	FString CurBullet = FString::FromInt(cur); // cur을 문자열로 변환
	FString AllBullet = FString::FromInt(all); // all을 문자열로 변환

	FString BulletText = FString::Printf(TEXT("%s / %s"), *CurBullet, *AllBullet);

	//BulletStack_Text->SetFont(FSlateFontInfo(BulletStack_Text->Font.TypefaceFontName, 15));
	BulletStack_Text->SetText(FText::FromString(BulletText));
}

	
