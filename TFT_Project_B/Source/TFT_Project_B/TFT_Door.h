// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_Door.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_Door : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATFT_Door();

	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	bool bIsOpen;

};
