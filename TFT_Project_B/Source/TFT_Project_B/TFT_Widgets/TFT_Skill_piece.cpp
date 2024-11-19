
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
	// air bon
	_brushCloseE_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Airbon_icon.Airbon_icon'"));
	// laser
	_brushCloseQ_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/laser_icon.laser_icon'"));
	// ??
	_brushCloseE_Resource = LoadObject<UTexture2D>(nullptr, TEXT(""));

	_nullBrush_Resource = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/invisible.invisible'"));

	_brushCloseQ.SetResourceObject(_brushCloseQ_Resource);
	_brushCloseE.SetResourceObject(_brushCloseE_Resource);
	_brushLongQ.SetResourceObject(_brushLongQ_Resource);
	_brushLongE.SetResourceObject(_brushLongE_Resource);

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
}

void UTFT_Skill_piece::SetSlotImg(WEAPON_TYPE itemtype, int32 socket)
{
	switch (itemtype)
	{
	case closeRange:
	{
		if (socket == 1)
		{

		}
		else if (socket == 2)
		{

		}
	}
		break;
	case longLange:
	{
		if (socket == 1)
		{

		}
		else if (socket == 2)
		{

		}
	}
		break;
	default:
		break;
	}
}

void UTFT_Skill_piece::ResetSlotImg()
{
	SkillSlotImg->SetBrush(_nullBrush);
}

void UTFT_Skill_piece::SetSlotPBPercent(float percent)
{
	SkillSlotPB->SetPercent(percent);
}

void UTFT_Skill_piece::SetCDT(float CDT)
{
	_coolDownTime = CDT;
}
