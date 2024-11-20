
#include "TFT_Widgets/TFT_Skill_piece.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Slate\SlateBrushAsset.h"

#include "Kismet/KismetMathLibrary.h"

void UTFT_Skill_piece::NativeConstruct()
{
	Super::NativeConstruct();

	// rush
	_brushCloseQ_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Rush_icon.Rush_icon'"));
	_brushCloseQ_Black_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Rush_Black_icon.Rush_Black_icon'"));
	// air bon
	_brushCloseE_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Airbon_icon.Airbon_icon'"));
	_brushCloseE_Black_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Airbon_Black_icon.Airbon_Black_icon'"));
	// laser
	_brushLongQ_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/laser_icon.laser_icon'"));
	_brushLongQ_Black_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/laser_Black_icon.laser_Black_icon'"));
	// ??
	_brushLongE_Resource = LoadObject<UTexture2D>(nullptr, TEXT(""));
	_brushLongE_Black_Resource = LoadObject<UTexture2D>(nullptr, TEXT(""));

	_nullBrush_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/invisible.invisible'"));

	_brushCloseQ.SetResourceObject(_brushCloseQ_Resource);
	_brushCloseQ_Black.SetResourceObject(_brushCloseQ_Black_Resource);
	_brushCloseE.SetResourceObject(_brushCloseE_Resource);
	_brushCloseE_Black.SetResourceObject(_brushCloseE_Black_Resource);
	_brushLongQ.SetResourceObject(_brushLongQ_Resource);
	_brushLongQ_Black.SetResourceObject(_brushLongQ_Black_Resource);
	_brushLongE.SetResourceObject(_brushLongE_Resource);
	_brushLongE_Black.SetResourceObject(_brushLongE_Black_Resource);

	_nullBrush.SetResourceObject(_nullBrush_Resource);

	SkillSlotImg->SetBrush(_nullBrush);
}

void UTFT_Skill_piece::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_runTime >= _coolDownTime)
	{
		_runTime = 0.0f;
		bCoolDownOn = false;

		return;
	}

	if (bCoolDownOn)
	{
		_runTime += InDeltaTime;

		float pbPercent = 1.0f - UKismetMathLibrary::NormalizeToRange(_runTime, 0.0f, _coolDownTime);

		SetSlotPBPercent(pbPercent);
	}
}

void UTFT_Skill_piece::SetSlotNumText(FString text)//
{

	SkillSlotNumText->SetText(FText::FromString(*text));
}

void UTFT_Skill_piece::SetSlotImg(WEAPON_TYPE itemtype, int32 socket)
{
	switch (itemtype)
	{
	case closeRange:
	{
		if (socket == 1)
		{
			SkillSlotPB->WidgetStyle.BackgroundImage = _brushCloseQ;
			SkillSlotPB->WidgetStyle.FillImage = _brushCloseQ_Black;
		}
		else if (socket == 2)
		{
			SkillSlotPB->WidgetStyle.BackgroundImage = _brushCloseE;
			SkillSlotPB->WidgetStyle.FillImage = _brushCloseE_Black;
		}
	}
		break;
	case longLange:
	{
		if (socket == 1)
		{
			SkillSlotPB->WidgetStyle.BackgroundImage = _brushLongQ;
			SkillSlotPB->WidgetStyle.FillImage = _brushLongQ_Black;
		}
		else if (socket == 2)
		{
			SkillSlotPB->WidgetStyle.BackgroundImage = _brushLongE;
			SkillSlotPB->WidgetStyle.FillImage = _brushLongE_Black;
		}
	}
		break;
	default:
		break;
	}
}

void UTFT_Skill_piece::ResetSlotImg()
{
	SkillSlotPB->WidgetStyle.BackgroundImage = _nullBrush;
	SkillSlotPB->WidgetStyle.FillImage = _nullBrush;
}

void UTFT_Skill_piece::SetSlotPBPercent(float percent)
{
	SkillSlotPB->SetPercent(percent);
}

void UTFT_Skill_piece::SetCDT(float CDT)
{
	_coolDownTime = CDT;
}
