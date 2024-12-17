// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TFT_MonsterSpawnManager.generated.h"

class ATFT_BossMonster_Grux;
class ATFT_BossMonster_Rampage;
class ATFT_NormalMonster_Rampage;
class ATFT_Boss_BJ;
class ATFT_NormalMonster_BJ;

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

	bool IsAllCleared();

	void AddTimerHandleInArray(FTimerHandle* timerHandle);

	void PauseAllTimerHandle();

private:
	template <typename MonsterType>
	void ConstructMonsterSubclass(TSubclassOf<MonsterType> &subclass, FString path);

	void ConstructMonsterMesh(TObjectPtr<USkeletalMesh> &mesh, FString path);

	template <typename MonsterAnim>
	void ConstructMonsterAnimInst(TSubclassOf<MonsterAnim>& subclass, FString path);

	template <typename MonsterType>
	void CreateMonster(TSubclassOf<MonsterType>& subclass, TArray<MonsterType*>& mArray, int32 num);

	template <typename MonsterType, typename MonsterAnim>
	void SpawnMonster(TArray<MonsterType*> mArray, TSubclassOf<MonsterAnim> anim, TObjectPtr<USkeletalMesh> mesh, int32 level);

	template <typename MonsterType, typename MonsterAnim>
	void SetSpawnTimer(TArray<MonsterType*> &mArray, TSubclassOf<MonsterAnim>& anim, TObjectPtr<USkeletalMesh>& mesh, int32 level, FTimerHandle& timerHandle, float inRate, bool bLoop, bool timerPause);

	template <typename MonsterType, typename MonsterAnim>
	void ChangeSpawnTimer(TArray<MonsterType*>& mArray, TSubclassOf<MonsterAnim>& anim, TObjectPtr<USkeletalMesh>& mesh, int32 level, FTimerHandle& timerHandle, float inRate, bool bLoop, bool timerPause);

	void SetSpawnPos();

	template <typename MonsterType, typename MonsterAnim>
	void SurroundSpawn(TArray<MonsterType*> mArray, TSubclassOf<MonsterAnim> anim, TObjectPtr<USkeletalMesh> mesh, int32 level, int32 num, float dist);

private:
	float _playTime = 0.0f;

	AActor* _player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnPos, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> _spawnPositions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnPos, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _spawnPosClass;
	int32 _randSpawnNum = 0;

	FVector _spawnPosVector1 = FVector(-3200.0f, -2800.0f, 100.0f); FVector _spawnPosVector2 = FVector(-3200.0f, 2800.0f, 100.0f);
	FVector _spawnPosVector3 = FVector(3200.0f, 2800.0f, 100.0f); FVector _spawnPosVector4 = FVector(3200.0f, -2800.0f, 100.0f);

	// Stages
	bool bOnStage_1 = false; bool bOnStage_2 = false; bool bOnStage_3 = false; bool bOnStage_4 = false;
	bool bOnStage_5 = false; bool bOnStage_6 = false; bool bOnStage_7 = false; bool bOnStage_8 = false;
	bool bOnStage_9 = false; bool bOnStage_10 = false; bool bOnStage_11 = false; bool bOnStage_12 = false;
	bool bOnEndStage = false;

	// Events
	bool bSrdSpawn_1 = false; bool bSrdSpawn_2 = false; bool bSrdSpawn_3 = false; bool bSrdSpawn_4 = false;

	TArray<FTimerHandle*> _timerHandles;

#pragma region Monsters

	// Grux_Normal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grux, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_BossMonster_Grux> _gruxClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grux, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_Grux> _gruxAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grux, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_BossMonster_Grux*> _gruxArray;

	FTimerHandle _gruxSpawnTimerHandle;

	// Rampage_Boss
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_BossMonster_Rampage> _rampageBossClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_Rampage> _rampageBossAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_BossMonster_Rampage*> _rampageBossArray;

	FTimerHandle _rampageBossSpawnTimerHandle;

	// Rampage_Normal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_NormalMonster_Rampage> _rampageNormalClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_NormalRampage> _rampageNormalAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_NormalMonster_Rampage*> _rampageNormalArray;

	FTimerHandle _rampageNormalSpawnTimerHandle;

	// BJ_Boss
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_Boss_BJ> _bjBossClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_BJ> _bjBossAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_Boss_BJ*> _bjBossArray;

	FTimerHandle _bjBossSpawnTimerHandle;

	// BJ_Normal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_NormalMonster_BJ> _bjNormalClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTFT_AnimInstance_NormalBJ> _bjNormalAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_NormalMonster_BJ*> _bjNormalArray;

	FTimerHandle _bjNormalSpawnTimerHandle;

#pragma endregion

#pragma region Meshes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grux, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _gruxMesh_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grux, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _gruxMesh_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grux, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _gruxMesh_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _rampageMesh_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _rampageMesh_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _rampageMesh_3;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rampage, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<USkeletalMesh> _rampageMesh_4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BJ, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _bjMesh_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BJ, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> _bjMesh_2;

#pragma endregion
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

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::CreateMonster(TSubclassOf<MonsterType>& subclass, TArray<MonsterType*>& mArray, int32 num)
{
	for (int32 i = 0; i < num; i++)
	{
		auto actor = GetWorld()->SpawnActor(subclass);
		
		if (actor->IsValidLowLevel())
		{
			MonsterType* monster = Cast<MonsterType>(actor);		

			mArray.Add(monster);
			monster->DeActive();
		}
	}
}

template<typename MonsterType, typename MonsterAnim>
inline void ATFT_MonsterSpawnManager::SpawnMonster(TArray<MonsterType*> mArray, TSubclassOf<MonsterAnim> anim, TObjectPtr<USkeletalMesh> mesh, int32 level)
{
	for (auto monster : mArray)
	{
		if (monster->IsValidLowLevel())
		{
			if (monster->bIsSpawned) continue;

			// monster->PreActive();
			monster->ChangeMesh(mesh);
			monster->GetMesh()->SetAnimInstanceClass(anim);
			monster->SetAnimInstanceBind();

			_randSpawnNum = FMath::RandRange(0, 3);
			monster->SetActorLocation(_spawnPositions[_randSpawnNum]->GetActorLocation());
			monster->ChangeLevel(level);
			monster->Active();

			return;
		}
	}
}

template<typename MonsterType, typename MonsterAnim>
inline void ATFT_MonsterSpawnManager::SetSpawnTimer
(TArray<MonsterType*> &mArray, TSubclassOf<MonsterAnim>& anim, TObjectPtr<USkeletalMesh>& mesh, int32 level, FTimerHandle& timerHandle, float inRate, bool bLoop, bool timerPause)
{
	if(timerPause) GetWorldTimerManager().PauseTimer(timerHandle);

	FTimerDelegate dlgt = FTimerDelegate::CreateUObject(this, &ATFT_MonsterSpawnManager::SpawnMonster, mArray, anim, mesh, level);

	GetWorldTimerManager().SetTimer(timerHandle, dlgt, inRate, bLoop);
	
}

template<typename MonsterType, typename MonsterAnim>
inline void ATFT_MonsterSpawnManager::ChangeSpawnTimer(TArray<MonsterType*>& mArray, TSubclassOf<MonsterAnim>& anim, TObjectPtr<USkeletalMesh>& mesh, int32 level, FTimerHandle& timerHandle, float inRate, bool bLoop, bool timerPause)
{
	SetSpawnTimer(mArray, anim, mesh, level, timerHandle, inRate, bLoop, timerPause);
}

template<typename MonsterType, typename MonsterAnim>
inline void ATFT_MonsterSpawnManager::SurroundSpawn(TArray<MonsterType*> mArray, TSubclassOf<MonsterAnim> anim, TObjectPtr<USkeletalMesh> mesh, int32 level, int32 num, float dist)
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

				// monster->PreActive();
				monster->ChangeMesh(mesh);
				monster->GetMesh()->SetAnimInstanceClass(anim);
				monster->SetAnimInstanceBind();

				monster->SetActorLocation(srdPos);
				monster->ChangeLevel(level);
				monster->Active();

				break;
			}
		}
	}
}
