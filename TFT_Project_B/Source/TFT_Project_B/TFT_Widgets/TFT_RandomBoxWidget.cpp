// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Widgets/TFT_RandomBoxWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool UTFT_RandomBoxWidget::Initialize()
{
	bool abc = Super::Initialize();


	if (UButton* LeftButton = Cast<UButton>(GetWidgetFromName(TEXT("Select_Left"))))
	{
		LeftButton->OnClicked.AddDynamic(this, &UTFT_RandomBoxWidget::SelectLeftEvent);
	}
	if (UButton* RightButton = Cast<UButton>(GetWidgetFromName(TEXT("Select_Right"))))
	{
		RightButton->OnClicked.AddDynamic(this, &UTFT_RandomBoxWidget::SelectRightEvent);
	}

	_InvisibleImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Choice.Choice'"));
	_PlusHpImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/maxHpPlusIcon.maxHpPlusIcon'"));
	_PlusBarrierImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/BarrierIcon.BarrierIcon'"));
	_AddAmmoImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/AmmoIcon.AmmoIcon'"));

	_HPrecrveryImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/Healing.Healing'"));
	_SupplyImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/AirDrop.AirDrop'"));
	_BarrierRecrveryImg = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/Widget/Widget_textrue/BarrierFull.BarrierFull'"));


	RandomBoxSetting(1);
	RandomBoxSetting(2);

	return abc;
}

void UTFT_RandomBoxWidget::RandomBoxSetting(int32 num)
{
	int32 randoms = FMath::RandRange(1, 4);

	if (randoms == 1) // Max HP Plus
	{
		int32 plusNum = FMath::RandRange(1, 5) * 10;

		if (num == 1)
		{
			RandomImg_Left->SetBrushFromTexture(_PlusHpImg);
			_LeftValue = 1;
			_LeftPlusValue = plusNum;

			FString HPstring = FString::Printf(TEXT("Max HP + %d"), _LeftPlusValue);
			LeftText->SetText(FText::FromString(HPstring));
		}
		else if(num == 2)
		{
			RandomImg_Right->SetBrushFromTexture(_PlusHpImg);
			_RightValue = 1;
			_RightPlusValue = plusNum;

			FString HPstring = FString::Printf(TEXT("Max HP + %d"), _RightPlusValue);
			RightText->SetText(FText::FromString(HPstring));
		}
	}
	else if (randoms == 2) // Max Barrier Plus
	{
		int32 plusNum = FMath::RandRange(1, 5) * 10;

		if (num == 1)
		{
			RandomImg_Left->SetBrushFromTexture(_PlusBarrierImg);
			_LeftValue = 2;
			_LeftPlusValue = plusNum;

			FString Barrierstring = FString::Printf(TEXT("Max Barrier + %d"), _LeftPlusValue);
			LeftText->SetText(FText::FromString(Barrierstring));
		}
		else if (num == 2)
		{
			RandomImg_Right->SetBrushFromTexture(_PlusBarrierImg);
			_RightValue = 2;
			_RightPlusValue = plusNum;

			FString Barrierstring = FString::Printf(TEXT("Max Barrier + %d"), _RightPlusValue);
			RightText->SetText(FText::FromString(Barrierstring));
		}
	}
	else if (randoms == 3) // Max Roader Plus
	{
		int32 plusNum = FMath::RandRange(1, 5);

		if (num == 1)
		{
			RandomImg_Left->SetBrushFromTexture(_AddAmmoImg);
			_LeftValue = 3;
			_LeftPlusValue = plusNum;

			FString Ammostring = FString::Printf(TEXT("Max Loader + %d"), _LeftPlusValue);
			LeftText->SetText(FText::FromString(Ammostring));
		}
		else if (num == 2)
		{
			RandomImg_Right->SetBrushFromTexture(_AddAmmoImg);
			_RightValue = 3;
			_RightPlusValue = plusNum;

			FString Ammostring = FString::Printf(TEXT("Max loader + %d"), _RightPlusValue);
			RightText->SetText(FText::FromString(Ammostring));
		}
	}//////////////////////////////////////////////// 여기까지만 구현된상태
	else if (randoms == 4) // cur bullet Supply
	{
		int32 plusNum = FMath::RandRange(1, 5) * 30;

		if (num == 1)
		{
			RandomImg_Left->SetBrushFromTexture(_SupplyImg);
			_LeftValue = 4;
			_LeftPlusValue = plusNum;

			FString Ammostring = FString::Printf(TEXT("Supplt : Ammo + %d"), _LeftPlusValue);
			LeftText->SetText(FText::FromString(Ammostring));
		}
		else if (num == 2)
		{
			RandomImg_Right->SetBrushFromTexture(_SupplyImg);
			_RightValue = 4;
			_RightPlusValue = plusNum;

			FString Ammostring = FString::Printf(TEXT("Supplt : Ammo + %d"), _RightPlusValue);
			RightText->SetText(FText::FromString(Ammostring));
		}
	}

}

void UTFT_RandomBoxWidget::SelectLeftEvent()
{
	_LeftEvent.Broadcast(_LeftValue, _LeftPlusValue);

	UE_LOG(LogTemp, Log, TEXT("LeftButton Event"));
	_SelectEvent.Broadcast(); // close
}

void UTFT_RandomBoxWidget::SelectRightEvent()
{
	_RightEvent.Broadcast(_RightValue, _RightPlusValue);

	UE_LOG(LogTemp, Log, TEXT("RightButton Event"));
	_SelectEvent.Broadcast(); // close
}
