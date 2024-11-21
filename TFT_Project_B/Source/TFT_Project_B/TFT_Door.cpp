// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATFT_Door::ATFT_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 문 메시
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;


	// 초기 상태
	bIsOpen = false;
}

// Called when the game starts or when spawned
void ATFT_Door::BeginPlay()
{
	Super::BeginPlay();
	

}

void ATFT_Door::OpenDoor()
{
	if (!bIsOpen)
	{
		bIsOpen = true;
		FVector OpenPosition = GetActorLocation() + FVector(0, 0, 400); // Z축으로 400 이동
		SetActorLocation(OpenPosition);
	}
}

void ATFT_Door::CloseDoor()
{
	if (bIsOpen)
	{
		bIsOpen = false;
		FVector ClosedPosition = GetActorLocation() - FVector(0, 0, 400); // Z축으로 400 이동
		SetActorLocation(ClosedPosition);
	}
}

// Called every frame
void ATFT_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

