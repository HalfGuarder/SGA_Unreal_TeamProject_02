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

void ATFT_MonsterSpawnManager::BeginDestroy()
{
	Super::BeginDestroy();

	// _gruxSpawnTimerHandle.Invalidate();
	// _rampageBossSpawnTimerHandle.Invalidate();
}

void ATFT_MonsterSpawnManager::BeginPlay()
{
	Super::BeginPlay();

}

void ATFT_MonsterSpawnManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_gruxArray.Empty();
	_rampageBossArray.Empty();

	CreateMonster(_gruxClass, _gruxArray, _gruxAnimClass, _gruxMesh, 10);
	CreateMonster(_rampageBossClass, _rampageBossArray, _rampageBossAnimClass, _rampageBossMesh, 2);

	SetSpawnTimer(_gruxArray, _gruxSpawnTimerHandle, 3.0f, true);
	SetSpawnTimer(_rampageBossArray,  _rampageBossSpawnTimerHandle, 10.0f, true);
}

void ATFT_MonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
