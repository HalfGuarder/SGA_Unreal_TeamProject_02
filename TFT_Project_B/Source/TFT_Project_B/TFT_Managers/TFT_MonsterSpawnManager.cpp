#include "TFT_Managers/TFT_MonsterSpawnManager.h"

#include "TimerManager.h"

#include "TFT_Boss_AIController.h"

#include "TFT_BossMonster_Grux.h"
#include "TFT_AnimInstance_Grux.h"

#include "TFT_BossMonster_Rampage.h"
#include "TFT_AnimInstance_Rampage.h"

ATFT_MonsterSpawnManager::ATFT_MonsterSpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Grux
	ConstructMonsterSubclass(_gruxClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_Grux/TFT_BossMonster_Grux_BP.TFT_BossMonster_Grux_BP_C'");
	ConstructMonsterMesh(_gruxMesh, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_2/Grux_Beetle_Red/Meshes/GruxBeetleRed.GruxBeetleRed'");
	ConstructMonsterAnimInst(_gruxAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_BossMonster_Grux_AnimInstance_BP.TFT_BossMonster_Grux_AnimInstance_BP_C'");

	// Rampage_Boss
	ConstructMonsterSubclass(_rampageBossClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_DH/TFT_BossMonster_Rampage.TFT_BossMonster_Rampage_C'");
	ConstructMonsterMesh(_rampageBossMesh, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");
	ConstructMonsterAnimInst(_rampageBossAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_AnimInstanceRampage_BP.TFT_AnimInstanceRampage_BP_C'");

}

//void ATFT_MonsterSpawnManager::CreateMonster(TSubclassOf<ATFT_BossMonster_Grux> subclass, TArray<ATFT_BossMonster_Grux*> mArray, int32 num)
//{
//	for (int32 i = 0; i < num; i++)
//	{
//		auto actor = GetWorld()->SpawnActor(subclass);
//
//		ATFT_BossMonster_Grux* monster = Cast<ATFT_BossMonster_Grux>(actor);
//
//		mArray.Add(monster);
//		monster->DeActive();
//	}
//}

void ATFT_MonsterSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	CreateMonster(_gruxClass, _gruxArray, 10);
	CreateMonster(_rampageBossClass, _rampageBossArray, 2);
	
	SetSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh, _gruxSpawnTimerHandle, _gruxSpawnTimerDelegate, 3.0f, true);
	SetSpawnTimer(_rampageBossArray, _rampageBossAnimClass, _rampageBossMesh, _rampageBossSpawnTimerHandle, _rampageBossSpawnTimerDelegate, 10.0f, true);
}

void ATFT_MonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*_gruxSpawnTimer += DeltaTime;

	if (_gruxSpawnTimer >= 3.0f)
	{
		SpawnMonster(_gruxArray, _gruxAnimClass, _gruxMesh);
		_gruxSpawnTimer = 0;
	}*/

}

void ATFT_MonsterSpawnManager::ConstructMonsterMesh(TObjectPtr<USkeletalMesh> &mesh, FString path)
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mMesh
	(*path);
	if (mMesh.Succeeded())
	{
		mesh = mMesh.Object;
	}
}
