// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_SceneManager.generated.h"

UENUM()
enum class SceneType : int32
{
	Tutorial,
	FirstMap,
};

UCLASS()
class TFT_PROJECT_B_API ATFT_SceneManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATFT_SceneManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
