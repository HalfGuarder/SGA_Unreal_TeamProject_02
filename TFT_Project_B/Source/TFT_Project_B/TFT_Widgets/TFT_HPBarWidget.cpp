// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_HPBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


//include "Engine/Texture2D.h"


void UTFT_HPBarWidget::NativeConstruct()
{
	profile_empty = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/invisible.invisible'"));

	profile_player = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/player_profile_pro.player_profile_pro'"));

	profile_boss1 = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Boss_Rampage_profile.Boss_Rampage_profile'"));

	profile_boss2 = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Boss_swordman_profile.Boss_swordman_profile'"));

	profile->SetBrushFromTexture(profile_empty);
}

void UTFT_HPBarWidget::SetHpBarValue(float ratio)
{
	BP_HpBar->SetPercent(ratio);
}

void UTFT_HPBarWidget::SetBarrierBarValue(float ratio)
{
	BP_BarrierBar->SetPercent(ratio);
}

void UTFT_HPBarWidget::SetProfileImage(ProfileType num)
{
	switch (num)
	{
	case EMPTY:
	{
		profile->SetBrushFromTexture(profile_empty);
	}
		break;
	case PLAYER:
	{
		profile->SetBrushFromTexture(profile_player);
	}
		break;
	case BOSS1:
	{
		profile->SetBrushFromTexture(profile_boss1);
	}
		break;
	case BOSS2:
	{
		profile->SetBrushFromTexture(profile_boss2);
	}
		break;
	default:
		break;
	}
}

void UTFT_HPBarWidget::SetHpText(int32 maxHp)
{
	FString hpstring = FString::Printf(TEXT("%d"), maxHp);

	CurHp_Text->SetText(FText::FromString(hpstring));
	MaxHp_Text->SetText(FText::FromString(hpstring));
}

void UTFT_HPBarWidget::CurHpText(int32 curHp)
{
	if (curHp < 0) curHp = 0;

	FString hpstring = FString::Printf(TEXT("%d"), curHp);

	CurHp_Text->SetText(FText::FromString(hpstring));
}

void UTFT_HPBarWidget::SetBarrierText(int32 Barrier)
{
	if (Barrier == 0)
	{
		CurBarrierText->SetText(_nullText);
		return;
	}

	FString Barrierstring = FString::Printf(TEXT("%d"), Barrier);

	CurBarrierText->SetText(FText::FromString(Barrierstring));

}

