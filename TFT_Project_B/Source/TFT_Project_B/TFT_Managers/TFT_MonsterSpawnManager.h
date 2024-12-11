// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void SpawnMonster(TArray<MonsterType*> mArray);

	template <typename MonsterType>
	void SetSpawnTimer(TArray<MonsterType*> &mArray, FTimerHandle timerhandle, float inRate, bool bLoop);

private:
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
inline void ATFT_MonsterSpawnManager::SpawnMonster(TArray<MonsterType*> mArray)
{
	for (auto monster : mArray)
	{
		if (monster->IsValidLowLevel())
		{
			if (monster->bIsSpawned) continue;

			// temp
			monster->SetActorLocation(FVector(100.0f, 100.0f, 100.0f));
			monster->Active();

			return;
		}
	}
}

template<typename MonsterType>
inline void ATFT_MonsterSpawnManager::SetSpawnTimer
(TArray<MonsterType*> &mArray, FTimerHandle timerhandle, float inRate, bool bLoop)
{
	FTimerDelegate dlgt = FTimerDelegate::CreateUObject(this, &ATFT_MonsterSpawnManager::SpawnMonster, mArray);

	GetWorldTimerManager().SetTimer(timerhandle, dlgt, inRate, bLoop);
	
}
