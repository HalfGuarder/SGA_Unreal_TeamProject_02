// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_InvenComponent.h"

#include "TFT_Item.h"
#include "TFT_Player.h"
#include "TFT_UIManager.h"

UTFT_InvenComponent::UTFT_InvenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	_items.SetNum(_inventoryMaxSize);
}

void UTFT_InvenComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTFT_InvenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTFT_InvenComponent::AddItem(ATFT_Item* item)
{
	if (item->GetItemType() == "gold")
	{
		AddPlayerGold(item->GetItemGold());
		item->Disable();

		return;
	}
	else if (item->GetItemType() == "Bullet")
	{
		AddPlayerBullet(item->GetItemSpace());
		item->Disable();
	}
	else
	{
		for (int32 i = 0; i < _inventoryMaxSize; ++i)
		{
			if (_items.IsValidIndex(i) && _items[i] == nullptr)
			{
				_items[i] = item;
				_itemAddedEvent.Broadcast(item, i);
				item->Disable();
				return;
			}
		}
	}
}

void UTFT_InvenComponent::DropItem(int32 index)
{
	float randFloat = FMath::FRandRange(0, PI * 2.0f);

	float X = cosf(randFloat) * 300.0f;
	float Y = sinf(randFloat) * 300.0f;
	FVector playerPos = GetOwner()->GetActorLocation();
	FVector itemPos = playerPos + FVector(X, Y, 0.0f);
	_items[index]->SetItemPos(itemPos);
	_items[index] = nullptr;
}

void UTFT_InvenComponent::SellItem(int32 index)
{
	AddPlayerGold(_items[index]->GetSellGold());

	ATFT_Item* itemToRemove = _items[index];
	_items[index] = nullptr;
}

void UTFT_InvenComponent::UseItem(int32 index)
{
	_items[index] = nullptr;
}

void UTFT_InvenComponent::SlectItemUI(int32 index)
{
	_itemSlectEvent.Broadcast(_items[index], index);
}

void UTFT_InvenComponent::AddPlayerGold(int32 gold)
{
	_playerGold += gold;
	_GoldChangeEvnet.Broadcast(_playerGold);
}

void UTFT_InvenComponent::SetWeapon(ATFT_Item* NewWeapon)
{
	if (NewWeapon == nullptr) return;

	auto player = Cast<ATFT_Player>(GetOwner());

	FName HR_WeaponSocket(TEXT("hand_r_socket"));

	NewWeapon->Disable();

	if (_currentWeapon != nullptr)
	{
		// _currentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// _currentWeapon->SetOwner(nullptr);
		DisarmWeapon(_currentWeapon);
	}

	_currentWeapon = NewWeapon;
	_currentWeapon->SetActorHiddenInGame(false);
	_currentWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HR_WeaponSocket);
	_currentWeapon->SetOwner(player);

	_pairWeaponUIDelegate.Broadcast();

	if (_currentWeapon->_Itemid == 1)
	{
		//UIMANAGER->GetSkillUI()->SetSkillSlot(0, 5.0f, _currentWeapon->_Itemid);
		//UIMANAGER->GetSkillUI()->ResetSkillSlot(1);
	}
	else if (_currentWeapon->_Itemid == 2)
	{
		//UIMANAGER->GetSkillUI()->SetSkillSlot(1, 8.0f, _currentWeapon->_Itemid);
		//UIMANAGER->GetSkillUI()->ResetSkillSlot(0);
	}
}

void UTFT_InvenComponent::DisarmWeapon(ATFT_Item* curWeapon)
{
	if (curWeapon == nullptr) return;

	_spareWeapon = curWeapon;

	auto player = Cast<ATFT_Player>(GetOwner());

	FName S1_SwordSocket(TEXT("spine_01_sword_socket")); // 1
	FName TL_RifleSocket(TEXT("thigh_l_rifle_socket")); // 2

	if (curWeapon->GetItemID() == 1)
	{
		curWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, S1_SwordSocket);
	}
	if (curWeapon->GetItemID() == 2)
	{
		curWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TL_RifleSocket);
	}
}

void UTFT_InvenComponent::AddPlayerBullet(int32 Bullet)
{
	_Bullet += Bullet;
	_BulletEvent.Broadcast(_curBullet, _Bullet);
}

bool UTFT_InvenComponent::UseBullet()
{
	if (_curBullet <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("no bullets"));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BANG~"));
		_curBullet--;
		_BulletEvent.Broadcast(_curBullet, _Bullet);
		return true;
	}
}

void UTFT_InvenComponent::ReLoadBullet()
{
	int32 ammo = _ReLoadCount - _curBullet;

	if (_Bullet <= 0) // 탄이 아예없다면 리턴
	{
		UE_LOG(LogTemp, Log, TEXT("ReLoad CRASH"));
		return;
	}
	if (_Bullet < ammo && 0 < _Bullet) // 1발이라도 있다면 그만큼만 장전
	{
		ammo = _Bullet;
		UE_LOG(LogTemp, Log, TEXT("Ammo's miss"));
	}


	_Bullet -= ammo;
	_curBullet += ammo;
	_BulletEvent.Broadcast(_curBullet, _Bullet);
	UE_LOG(LogTemp, Log, TEXT("ReLoad %d AMMO"), ammo);
}

void UTFT_InvenComponent::SetcurBullet()
{
	_BulletEvent.Broadcast(_curBullet, _Bullet);
}

void UTFT_InvenComponent::ChangeWeapon()
{
	SetWeapon(_spareWeapon);
}

