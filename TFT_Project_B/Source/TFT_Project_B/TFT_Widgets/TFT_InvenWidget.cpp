// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_InvenWidget.h"

#include "TFT_Item.h"
#include "TFT_InvenComponent.h"

#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "TFT_UIManager.h"

bool UTFT_InvenWidget::Initialize()
{
	bool abc = Super::Initialize();
	
	auto array = Inven_UniformGrid->GetAllChildren();
	int32 slotIndex = 0;

	for (auto slot : array)
	{
		auto btn = Cast<UButton>(slot);
		if (btn)
		{
			_slotBtns.Add(btn);


			switch (slotIndex)
			{
			case 0:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView0);
				break;
			}
			case 1:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView1);
				break;
			}
			case 2:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView2);
				break;
			}
			case 3:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView3);
				break;
			}
			case 4:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView4);
				break;
			}
			case 5:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView5);
				break;
			}
			case 6:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView6);
				break;
			}
			case 7:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView7);
				break;
			}
			case 8:
			{
				btn->OnClicked.AddDynamic(this, &UTFT_InvenWidget::SetToDetailView8);
				break;
			}
			default:
				break;
			}
		}
		slotIndex++;
	}


	Exit_Button->OnClicked.AddDynamic(this, &UTFT_InvenWidget::InvenOpenClose);

	Drop_Button->OnClicked.AddDynamic(this, &UTFT_InvenWidget::DropItem);

	Use_Button->OnClicked.AddDynamic(this, &UTFT_InvenWidget::UseItem);

	_UIsaveiteminfo.SetNum(9);

	_emptySlot = LoadObject<UTexture2D>
		(nullptr, TEXT("/Script/Engine.Texture2D'/Game/ParagonRampage/Characters/Global/MaterialLayers/Metal/T_LightMetal02.T_LightMetal02'"));

	Item_Information->SetText(_nullText);
	Inven_ItemMiniInfo->SetText(_nullText);
	Inven_ItemExplanation->SetText(_nullText);

	return abc;
}

void UTFT_InvenWidget::InvenOpenClose()
{
	_CloseInvenBtn.Broadcast();
}

void UTFT_InvenWidget::DropItem()
{
	if (Store_DropSellTextCheck() == true)
	{
		if (this_Index == -1)
		{
			UE_LOG(LogTemp, Log, TEXT("Your Not Choice Item"));
			return;
		}
		else
		{
			_SlotItemEvent.Broadcast(_this_Item, this_Index);

			SetItemSlot(_emptySlot, this_Index);
			DetailViewImg->SetBrushFromTexture(_emptySlot);
			DeleteUIItem(this_Index);
			Item_Information->SetText(_nullText);
			Inven_ItemMiniInfo->SetText(_nullText);
			Inven_ItemExplanation->SetText(_nullText);
		}
	}
	else
	{
		if (this_Index == -1)
		{
			UE_LOG(LogTemp, Log, TEXT("Your Not Choice Item"));
			return;
		}
		else
		{
			_itemSellEvent.Broadcast(_this_Item, this_Index);

			SetItemSlot(_emptySlot, this_Index);
			DetailViewImg->SetBrushFromTexture(_emptySlot);
			DeleteUIItem(this_Index);
			Item_Information->SetText(_nullText);
			Inven_ItemMiniInfo->SetText(_nullText);
			Inven_ItemExplanation->SetText(_nullText);

		}
	}
}

void UTFT_InvenWidget::UseItem()
{
	if (this_Index == -1)
	{
		UE_LOG(LogTemp, Log, TEXT("Your Not Choice Item"));
		return;
	}
	else
	{

		_itemUesEvent.Broadcast(_this_Item, this_Index);

		SetItemSlot(_emptySlot, this_Index);
		DetailViewImg->SetBrushFromTexture(_emptySlot);
		DeleteUIItem(this_Index);
		Item_Information->SetText(_nullText);
		Inven_ItemMiniInfo->SetText(_nullText);
		Inven_ItemExplanation->SetText(_nullText);
	}
}

void UTFT_InvenWidget::SetItemSlot(UTexture2D* texture, int32 slotIndex)
{
	_slotBtns[slotIndex]->WidgetStyle.Normal.SetResourceObject(texture);
}

void UTFT_InvenWidget::AddUiItem(ATFT_Item* item, int32 index)
{
	//UE_LOG(LogTemp, Log, TEXT("AddUIItem In"));
	if (!item)
	{
		UE_LOG(LogTemp, Log, TEXT("NO ITem"));
	}
	SetItemSlot(item->GetMyTexture(), index);
	_UIsaveiteminfo[index] = item;
}

void UTFT_InvenWidget::SelectSlotItem(int32 index)
{
	if (_UIsaveiteminfo[index] != nullptr)
	{
		_this_Item = _UIsaveiteminfo[index];
		this_Index = index;
		UISetItemName(index);
		UISetItemMiniInfo(index);
		UISetItemExplanation(index);
		DetailViewImg->SetBrushFromTexture(_this_Item->GetMyTexture());
	}
	else
	{
		DetailViewImg->SetBrushFromTexture(_emptySlot);
		Item_Information->SetText(_nullText);
		Inven_ItemMiniInfo->SetText(_nullText);
		Inven_ItemExplanation->SetText(_nullText);
		this_Index = -1;
		_this_Item = nullptr;
	}
}

void UTFT_InvenWidget::SetToDetailView0()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 1"));
	SelectSlotItem(0);
}

void UTFT_InvenWidget::SetToDetailView1()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 2"));
	SelectSlotItem(1);
}

void UTFT_InvenWidget::SetToDetailView2()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 3"));
	SelectSlotItem(2);
}

void UTFT_InvenWidget::SetToDetailView3()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 4"));
	SelectSlotItem(3);
}

void UTFT_InvenWidget::SetToDetailView4()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 5"));
	SelectSlotItem(4);
}

void UTFT_InvenWidget::SetToDetailView5()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 6"));
	SelectSlotItem(5);
}

void UTFT_InvenWidget::SetToDetailView6()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 7"));
	SelectSlotItem(6);
}

void UTFT_InvenWidget::SetToDetailView7()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 8"));
	SelectSlotItem(7);
}

void UTFT_InvenWidget::SetToDetailView8()
{
	UE_LOG(LogTemp, Log, TEXT("Slot : 9"));
	SelectSlotItem(8);
}

void UTFT_InvenWidget::DeleteUIItem(int32 index)
{
	_UIsaveiteminfo[index] = nullptr;
	_this_Item = nullptr;
	this_Index = -1;
}

void UTFT_InvenWidget::UISetItemName(int32 index)
{
	FString ItemNameString = FString::Printf(TEXT("%s"), *_UIsaveiteminfo[index]->GetItemName());
	FText ItemName = FText::FromString(ItemNameString);

	Item_Information->SetText(ItemName);
}

void UTFT_InvenWidget::UISetItemMiniInfo(int32 index)
{
	FString ItemMiniString = FString::Printf(TEXT("%s \n SellPrice : %d gold"), *_UIsaveiteminfo[index]->GetItemMiniInfo(), _UIsaveiteminfo[index]->GetSellGold());
	FText ItemMiniInfo = FText::FromString(ItemMiniString);

	Inven_ItemMiniInfo->SetText(ItemMiniInfo);
}

void UTFT_InvenWidget::UISetItemExplanation(int32 index)
{
	FString ItemExString = FString::Printf(TEXT("%s"), *_UIsaveiteminfo[index]->GetItemExplanation());
	FText ItemExplanation = FText::FromString(ItemExString);

	Inven_ItemExplanation->SetText(ItemExplanation);
}

void UTFT_InvenWidget::UIGold(int32 gold)
{
	FString InvenGoldString = FString::Printf(TEXT("%d Gold"), gold);
	FText InvenUIGolds = FText::FromString(InvenGoldString);

	Inven_UIGold->SetText(InvenUIGolds);
}

bool UTFT_InvenWidget::Store_DropSellTextCheck()
{
	if (storeCheke == true)//(GetWorld()->GetFirstPlayerController()->bShowMouseCursor == true)
	{
		FString String = FString::Printf(TEXT("Sell"));
		FText Sell = FText::FromString(String);

		TFT_InvenWidget_Drop_Text->SetText(Sell);
		return false;
	}
	else
	{
		FString String = FString::Printf(TEXT("Drop"));
		FText Drop = FText::FromString(String);

		TFT_InvenWidget_Drop_Text->SetText(Drop);
		return true;
	}
}
