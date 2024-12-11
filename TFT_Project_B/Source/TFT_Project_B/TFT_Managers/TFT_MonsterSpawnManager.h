// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TFT_MonsterSpawnManager.generated.h"

class ATFT_BossMonster_Grux;
class ATFT_BossMonster_Rampage;

UCLASS()
class TFT_PROJECT_B_API ATFT_MonsterSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFT_MonsterSpawnManager();
	virtual void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	template <typename MonsterType>
	void ConstructMonsterSubclass(TSubclassOf<MonsterType> &subclass, FString path);

	void ConstructMonsterMesh(TObjectPtr<USkeletalMesh> &mesh, FString path);

	template <typename MonsterAnim>
	void ConstructMonsterAnimInst(TSubclassOf<MonsterAnim>& subclass, FString path);

	template <typename MonsterType, typename MonsterAnim>
	void CreateMonster(TSubclassOf<MonsterType>& subclass, TArray<MonsterType*>& mArray, TSubclassOf<MonsterAnim>& anim, TObjectPtr<USkeletalMesh>& mesh, int32 num);

	template <typename MonsterType>
	void SpawnMonster(TArray<MonsterType*> mArray, int32 level);

	template <typename MonsterType>
	void SetSpawnTimer(TArray<MonsterType*> &mArray, int32 level, FTimerHandle timerHandle, float inRate, bool bLoop);

	template <typename MonsterType>
	void ChangeSpawnTimer(TArray<MonsterType*>& mArray, int32 level, FTimerHandle timerHandle, float inRate, bool bLoop);

	void SetSpawnPos();

	template <typename MonsterType>
	void SurroundSpawn(TArray<MonsterType*> mArray, int32 level, int32 num, float dist);

private:
	AActor* _player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnPos, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> _spawnPositions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnPos, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _spawnPosClass;
	int32 _randSpawnNum = 0;

	FVector _spawnPosVector1 = FVector(-3200.0f, -2800.0f, 100.0f); FVector _spawnPosVector2 = FVector(-3200.0f, 2800.0f, 100.0f);
	FVector _spawnPosVector3 = FVector(3200.0f, 2800.0f, 100.0f); FVector _spawnPosVector4 = FVector(3200.0f, -2800.0f, 100.0f);

	float _playTime = 0.0f;

	// Surround Spawn
	// TArray<FVector>

	// Stages
	bool bOnStage_1 = false; bool bOnStage_2 = false; bool bOnStage_3 = false; bool bOnStage_4 = false;

	// Events
	bool bSrdSpawn_1 = false;

	// Grux_Normal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_BossMonster_Grux> _gruxClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_Grux> _gruxAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _gruxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_BossMonster_Grux*> _gruxArray;

	FTimerHandle _gruxSpawnTimerHandle;

	// Rampage_Boss
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_BossMonster_Rampage> _rampageBossClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_Rampage> _rampageBossAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _rampageBossMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_BossMonster_Rampage*> _rampageBossArray;

	FTimerHandle _rampageBossSpawnTimerHandle;

};

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::ConstructMonsterSubclass(TSubclassOf<MonsterType> &subclass, FString path)
{
	ConstructorHelpers::FClassFinder<MonsterType> mClass
	(*path);
	if (mClass.Succeeded())
	{
		subclass = mClass.Class;
	}
}

template<typename MonsterAnim>
inline void ATFT_MonsterSpawnManager::ConstructMonsterAnimInst(TSubclassOf<MonsterAnim> &subclass, FString path)
{
	ConstructorHelpers::FClassFinder<MonsterAnim> animClass
	(*path);
	if (animClass.Succeeded())
	{
		subclass = animClass.Class;
	}
}

template<typename MonsterType, typename MonsterAnim>
inline void ATFT_MonsterSpawnManager::CreateMonster(TSubclassOf<MonsterType>& subclass, TArray<MonsterType*>& mArray, TSubclassOf<MonsterAnim>& anim, TObjectPtr<USkeletalMesh>& mesh, int32 num)
{
	for (int32 i = 0; i < num; i++)
	{
		auto actor = GetWorld()->SpawnActor(subclass);
		
		if (actor->IsValidLowLevel())
		{
			MonsterType* monster = Cast<MonsterType>(actor);

			monster->ChangeMesh(mesh);
			monster->GetMesh()->SetAnimInstanceClass(anim);
			monster->SetAnimInstanceBind();

			mArray.Add(monster);
			monster->DeActive();
		}
	}
}

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::SpawnMonster(TArray<MonsterType*> mArray, int32 level)
{
	for (auto monster : mArray)
	{
		if (monster->IsValidLowLevel())
		{
			if (monster->bIsSpawned) continue;

			_randSpawnNum = FMath::RandRange(0, 3);
			monster->SetActorLocation(_spawnPositions[_randSpawnNum]->GetActorLocation());
			monster->ChangeLevel(level);
			monster->Active();

			return;
		}
	}
}

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::SetSpawnTimer
(TArray<MonsterType*> &mArray, int32 level, FTimerHandle timerHandle, float inRate, bool bLoop)
{
	FTimerDelegate dlgt = FTimerDelegate::CreateUObject(this, &ATFT_MonsterSpawnManager::SpawnMonster, mArray, level);

	GetWorldTimerManager().SetTimer(timerHandle, dlgt, inRate, bLoop);
	
}

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::ChangeSpawnTimer(TArray<MonsterType*>& mArray, int32 level, FTimerHandle timerHandle, float inRate, bool bLoop)
{
	GetWorldTimerManager().ClearTimer(timerHandle);
	
	SetSpawnTimer(mArray, level, timerHandle, inRate, bLoop);
}

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::SurroundSpawn(TArray<MonsterType*> mArray, int32 level, int32 num, float dist)
{
	for (int32 i = 0; i < num; i++)
	{
		for (auto monster : mArray)
		{
			if (monster->IsValidLowLevel())
			{
				if (monster->bIsSpawned) continue;

				if (_player == nullptr) return;

				FVector pPos = _player->GetActorLocation();
				pPos.Z = 0.0f;

				double randRange = FMath::RandRange(0.0f, 360.0f);
				FVector randPos = pPos.RotateAngleAxis(randRange, FVector::ZAxisVector);
				randPos.Normalize();
				randPos *= dist;

				FVector sumPos = pPos + randPos;

				FVector srdPos = FVector(sumPos.X, sumPos.Y, 0.0f);

				monster->SetActorLocation(srdPos);
				monster->ChangeLevel(level);
				monster->Active();

				break;
			}
		}
	}
}
