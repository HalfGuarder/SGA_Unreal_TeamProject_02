// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Item.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
#include "TFT_Player.h"

ATFT_Item::ATFT_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));

	_meshComponent->SetupAttachment(RootComponent);
	_trigger->SetupAttachment(_meshComponent);

	_meshComponent->SetCollisionProfileName(TEXT("TFT_Item"));
	_trigger->SetCollisionProfileName(TEXT("TFT_Item"));
	_trigger->SetSphereRadius(60.0f);

	ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj
	(TEXT("/Script/Engine.DataTable'/Game/Data/TFT_ItemData_Table.TFT_ItemData_Table'"));

	if (DataTableObj.Succeeded())
	{
		ItemDataTable = DataTableObj.Object;
	}
}

void ATFT_Item::BeginPlay()
{
	Super::BeginPlay();

	LoadItemData();

	Init();

}

void ATFT_Item::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_trigger->OnComponentBeginOverlap.AddDynamic(this, &ATFT_Item::OnMyCharacterOverlap);

}

void ATFT_Item::OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto MyPlayer = Cast<ATFT_Player>(OtherActor);

	if (MyPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Item Name : %s, Attack : %d"), *_Name, _AttackPower);

		if(_ItemType == "Equipment") 
			MyPlayer->_invenCom->SetWeapon(this);
		else//else if(_ItemType == "Bullet") 
			MyPlayer->AddItemPlayer(this);
		

		
		
	}
}

void ATFT_Item::Init()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (_meshComponent)
	{
		//_meshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화
		//_meshComponent->SetEnableGravity(true);  // 중력 활성화

		//_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//_meshComponent->SetCollisionObjectType(ECC_PhysicsBody);

		//_meshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}
}

void ATFT_Item::Disable()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ATFT_Item::LoadItemData()
{
	if (ItemDataTable)
	{
		static const FString ContextString(TEXT("ItemData"));
		FTFT_ItemData* ItemData = ItemDataTable->FindRow<FTFT_ItemData>(FName(*FString::FromInt(_Itemid)), ContextString, true);
		if (ItemData)
		{

			_ItemType = ItemData->ItemType;
			_Name = ItemData->ItemName;
			_AttackPower = ItemData->AttackPower;
			_Defense = ItemData->Defense;
			_Buy = ItemData->Buy;
			_Sell = ItemData->Sell;
			_Space = ItemData->Space;
			_Explanation = ItemData->Explanation;
			_MiniInfo = ItemData->MiniInfo;
			_ItemRegion = ItemData->ItemRegion;

			if (ItemData->ItemMesh)
			{
				ItemMesh = ItemData->ItemMesh;
				_meshComponent->SetStaticMesh(ItemData->ItemMesh);
			}

			if (ItemData->ItemTexture)
			{
				_ItemTexture = ItemData->ItemTexture;
				UMaterialInstanceDynamic* MaterialInstance = _meshComponent->CreateAndSetMaterialInstanceDynamic(0);
				if (MaterialInstance)
				{
					MaterialInstance->SetTextureParameterValue("ItemTexture", ItemData->ItemTexture);
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Item Name : %s"), *_Name);
		}
	}
}

void ATFT_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_Item::SetItemPos(FVector pos, FRotator rot)
{
	Init();
	SetActorLocation(pos);
	SetActorRotation(rot);
}


void ATFT_Item::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATFT_Item, _Itemid))
	{
		LoadItemData();
		UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty called. ID changed to: %d"), _Itemid);
	}
}

