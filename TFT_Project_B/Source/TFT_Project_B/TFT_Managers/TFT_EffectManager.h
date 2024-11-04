// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_EffectManager.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_EffectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	

	ATFT_EffectManager();

protected:
	virtual void BeginPlay() override;

	void CreateParticleClass(FString name, FString path);

	void CreateEffect();
public:	
	
	virtual void Tick(float DeltaTime) override;

	void Play(FString name, int32 effectType, FVector location, FRotator rotator = FRotator::ZeroRotator);

private:
	int32 _poolCount = 5;

	UPROPERTY()
	TMap<FString, TSubclassOf<ATFT_Effect>> _classTable;

	TMap<FString, TArray<ATFT_Effect*>> _effectTable;

	UPROPERTY()
	class USceneComponent* _rootComponent;
};
