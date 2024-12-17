// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFT_InvenComponent.generated.h"

class ATFT_Item;
class UTFT_InvenWidget;


DECLARE_MULTICAST_DELEGATE_TwoParams(ItemAdded, ATFT_Item*, int itemIndex)
DECLARE_MULTICAST_DELEGATE_TwoParams(BulletEvent, int32 cur, int32 max)
DECLARE_MULTICAST_DELEGATE_OneParam(InvenGold, int gold)
DECLARE_MULTICAST_DELEGATE_OneParam(ItemEvent, ATFT_Item*)
DECLARE_MULTICAST_DELEGATE(InvenEvent);

UENUM(BlueprintType)
enum class MonsterType : uint8
{
	NONE,
	Normal,
	BOSS
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TFT_PROJECT_B_API UTFT_InvenComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UTFT_InvenComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItem(ATFT_Item* item);

	UFUNCTION()
	void DropItem(int32 index);
	UFUNCTION()
	void SellItem(int32 index);
	void UseItem(int32 index);

	void SlectItemUI(int32 index);

	int32 GetPlayerGold() { return _playerGold; }
	void AddPlayerGold(int32 gold);

	void SetWeapon(class ATFT_Item* NewWeapon);
	void DisarmWeapon(ATFT_Item* curWeapon);

	void ChangeWeapon();

	InvenEvent _pairWeaponUIDelegate;

	int32 GetPlayerBullet() { return _Bullet; }
	int32 GetPlayerCurBullet() { return _curBullet; }
	void AddPlayerBullet(int32 Bullet);
	void AddMaxRoadBullet(int32 amount);


	bool UseBullet();
	void ReLoadBullet();
	void SetcurBullet();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_Item*> _items;

	int32 _inventoryMaxSize = 9;
	int32 _playerGold = 100;

	int32 _Bullet = 30;
	int32 _curBullet = 10;
	int32 _ReLoadCount = 10;
public:

	InvenEvent _invenOpenDelegate;

	ItemAdded _itemAddedEvent;
	ItemAdded _itemSlectEvent;

	InvenGold _GoldChangeEvnet;
	BulletEvent _BulletEvent;

	ItemEvent _BuffGetDelegate;
	ItemEvent _RandomBoxGetDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	ATFT_Item* _currentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	ATFT_Item* _spareWeapon;

private:
	UPROPERTY()
	TSubclassOf<ATFT_Item> _itemClass;

	ATFT_Item* _MonsterDropItem;
public:
	void SetMonsterItem(int32 lineNum);

	void DropMonsterItem(FVector pos, MonsterType type);


};
