// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "TFT_Item.generated.h"

DECLARE_MULTICAST_DELEGATE(ItemAdd);

USTRUCT(BlueprintType)
struct FTFT_ItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Buy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Sell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Space;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Explanation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MiniInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemRegion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh;
};

UCLASS()
class TFT_PROJECT_B_API ATFT_Item : public AActor
{
	GENERATED_BODY()
	
public:
	ATFT_Item();

protected:
	virtual void BeginPlay() override;


	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

public:

	void Init();
	void Disable();

	void LoadItemData();

	FString GetItemName() { return _Name; }
	FString GetItemMiniInfo() { return _MiniInfo; }
	FString GetItemExplanation() { return _Explanation; }
	int32 GetItemAttackDamage() { return _AttackPower; }
	UTexture2D* GetMyTexture() { return _ItemTexture; }
	int32 GetItemGold() { return _Buy; }
	int32 GetSellGold() { return _Sell; }
	FString GetItemType() { return _ItemType; }
	FString GetItemRegion() { return _ItemRegion; }
public:
	virtual void Tick(float DeltaTime) override;

	void SetItemPos(FVector pos, FRotator rot = FRotator::ZeroRotator);

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* _meshComponent;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* _trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemID")
	int32 _Itemid = 0;

	void SetItemid(int32 num) { _Itemid = num;  LoadItemData(); }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
private:
	FString _ItemType;
	FString _Name;
	int32 _AttackPower;
	int32 _Defense;
	int32 _Buy;
	int32 _Sell;
	int32 _Space;
	FString _Explanation;
	UTexture2D* _ItemTexture;
	UStaticMesh* ItemMesh;
	FString _MiniInfo;
	FString _ItemRegion;
};
