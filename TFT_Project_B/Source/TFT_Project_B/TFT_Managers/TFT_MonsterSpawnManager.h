// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_MonsterSpawnManager.generated.h"

class ATFT_BossMonster_Grux;

UCLASS()
class TFT_PROJECT_B_API ATFT_MonsterSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFT_MonsterSpawnManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnMonster(TObjectPtr<USkeletalMesh> mesh);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_BossMonster_Grux> _monsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_Grux> _monsterAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_BossMonster_Grux*> _monsters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _monsterMesh;

	float _spawnTimer = 0.0f;
	
};
