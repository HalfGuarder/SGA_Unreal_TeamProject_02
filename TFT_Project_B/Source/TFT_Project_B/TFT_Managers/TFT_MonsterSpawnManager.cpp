#include "TFT_Managers/TFT_MonsterSpawnManager.h"

#include "TimerManager.h"

#include "TFT_Boss_AIController.h"

#include "TFT_BossMonster_Grux.h"
#include "TFT_AnimInstance_Grux.h"

#include "TFT_BossMonster_Rampage.h"
#include "TFT_AnimInstance_Rampage.h"

#include "TFT_NormalMonster_Rampage.h"
#include "TFT_AnimInstance_NormalRampage.h"

#include "TFT_Boss_BJ.h"
#include "TFT_AnimInstance_BJ.h"

#include "TFT_NormalMonster_BJ.h"
#include "TFT_AnimInstance_NormalBJ.h"

ATFT_MonsterSpawnManager::ATFT_MonsterSpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor> sPos
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Spawn/TFT_SpawnPos_BP.TFT_SpawnPos_BP_C'"));
	if (sPos.Succeeded())
	{
		_spawnPosClass = sPos.Class;
	}

	// Grux_Normal
	ConstructMonsterSubclass(_gruxClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_Grux/TFT_BossMonster_Grux_BP.TFT_BossMonster_Grux_BP_C'");
	ConstructMonsterAnimInst(_gruxAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_BossMonster_Grux_AnimInstance_BP.TFT_BossMonster_Grux_AnimInstance_BP_C'");
	
	// Rampage_Boss
	ConstructMonsterSubclass(_rampageBossClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_DH/TFT_BossMonster_Rampage.TFT_BossMonster_Rampage_C'");
	ConstructMonsterAnimInst(_rampageBossAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_AnimInstanceRampage_BP.TFT_AnimInstanceRampage_BP_C'");
	
	// Rampage_Normal
	ConstructMonsterSubclass(_rampageNormalClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/NormalMonster_Rampage/TFT_NormalMonster_Rampage_BP.TFT_NormalMonster_Rampage_BP_C'");
	ConstructMonsterAnimInst(_rampageNormalAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/NormalMonster_Rampage/Normal_Rampage_Animation/TFT_AnimInstance_NormalRampage_BP.TFT_AnimInstance_NormalRampage_BP_C'");

	// BJ_Boss
	ConstructMonsterSubclass(_bjBossClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_BJ/MyTFT_Boss_BJ.MyTFT_Boss_BJ_C'");
	ConstructMonsterAnimInst(_bjBossAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_BJ/Animations/TFT_BossAnimInstanceBJ_BP.TFT_BossAnimInstanceBJ_BP_C'");
	
	// BJ_Normal
	ConstructMonsterSubclass(_bjNormalClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/Normal_Monster_BJ/TFT_NormalMonster_BJ_BP.TFT_NormalMonster_BJ_BP_C'");
	ConstructMonsterAnimInst(_bjNormalAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/Normal_Monster_BJ/NormalBJ_Animation/TFT_AnimInstance_NormalBJ_BP.TFT_AnimInstance_NormalBJ_BP_C'");
	
	// Mesh
	ConstructMonsterMesh(_gruxMesh_1, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux'");
	ConstructMonsterMesh(_gruxMesh_2, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_2/Grux_Beetle_Red/Meshes/GruxBeetleRed.GruxBeetleRed'");
	ConstructMonsterMesh(_gruxMesh_3, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_2/Grux_Beetle_Molten/Meshes/GruxMolten.GruxMolten'");
	ConstructMonsterMesh(_rampageMesh_1, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");
	ConstructMonsterMesh(_rampageMesh_2, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'");
	ConstructMonsterMesh(_rampageMesh_3, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier_1/Rampage_RedNeck/Mesh/Rampage_Redneck.Rampage_Redneck'");
	// ConstructMonsterMesh(_rampageMesh_4, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Tough/Mesh/Tough_LP_Master_ForVariants_Test.Tough_LP_Master_ForVariants_Test'");
	ConstructMonsterMesh(_bjMesh_1, "/Script/Engine.SkeletalMesh'/Game/Asset/SciFi_Soldiers/Mesh/Male/SK_SciFi_Soldier_Male_Skin4.SK_SciFi_Soldier_Male_Skin4'");
	ConstructMonsterMesh(_bjMesh_2, "/Script/Engine.SkeletalMesh'/Game/Asset/SciFi_Soldiers/Mesh/Male/SK_SciFi_Soldier_Male_Skin1.SK_SciFi_Soldier_Male_Skin1'");
}

void ATFT_MonsterSpawnManager::BeginDestroy()
{
	Super::BeginDestroy();

}

void ATFT_MonsterSpawnManager::BeginPlay()
{
	Super::BeginPlay();

}

void ATFT_MonsterSpawnManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetSpawnPos();

	_gruxArray.Empty();
	_rampageBossArray.Empty();

	CreateMonster(_gruxClass, _gruxArray, 150);
	CreateMonster(_rampageBossClass, _rampageBossArray, 15);
	CreateMonster(_rampageNormalClass, _rampageNormalArray, 120);
	CreateMonster(_bjBossClass, _bjBossArray, 15);
	CreateMonster(_bjNormalClass, _bjNormalArray, 50);

	AddTimerHandleInArray(&_gruxSpawnTimerHandle);
	AddTimerHandleInArray(&_rampageBossSpawnTimerHandle);
	AddTimerHandleInArray(&_rampageNormalSpawnTimerHandle);
	AddTimerHandleInArray(&_bjBossSpawnTimerHandle);
	AddTimerHandleInArray(&_bjNormalSpawnTimerHandle);

	// SetSpawnTimer(_gruxArray, _gruxSpawnTimerHandle, 3.0f, true);
	// SetSpawnTimer(_rampageBossArray,  _rampageBossSpawnTimerHandle, 10.0f, true);
}

void ATFT_MonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_playTime += DeltaTime;

	if (_playTime >= 1.0f && !bOnStage_1)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_1, 1, _gruxSpawnTimerHandle, 1.5f, true, true);
		bOnStage_1 = true;
	}

	if (_playTime >= 30.0f && !bOnStage_2)
	{
		_player = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		bOnStage_2 = true;
	}

	if (_playTime >= 50.0f && !bOnStage_3)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_2, 2, _gruxSpawnTimerHandle, 1.0f, true, true);
		ChangeSpawnTimer(_rampageNormalArray, _rampageNormalAnimClass, _rampageMesh_2, 2, _rampageNormalSpawnTimerHandle, 1.5f, true, true);
		bOnStage_3 = true;
	}

	if (_playTime >= 70.0f && !bSrdSpawn_1)
	{
		SurroundSpawn(_gruxArray, _gruxAnimClass, _gruxMesh_1, 1, 40, 900.0f);
		bSrdSpawn_1 = true;
	}

	if (_playTime >= 100.0f && !bOnStage_4)
	{
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		bOnStage_4 = true;
	}

	if (_playTime >= 120.0f && !bOnStage_5)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_2, 2, _gruxSpawnTimerHandle, 1.0f, true, true);

		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);

		bOnStage_5 = true;
	}

	if (_playTime >= 135.0f && !bSrdSpawn_2)
	{
		SurroundSpawn(_rampageNormalArray, _rampageNormalAnimClass, _rampageMesh_2, 2, 30, 900.0f);
		bSrdSpawn_2 = true;
	}

	if (_playTime >= 150.0f && !bOnStage_6)
	{
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		bOnStage_6 = true;
	}

	if (_playTime >= 170.0f && !bOnStage_7)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_3, 3, _gruxSpawnTimerHandle, 1.0f, true, true);
		ChangeSpawnTimer(_rampageNormalArray, _rampageNormalAnimClass, _rampageMesh_1, 3, _rampageNormalSpawnTimerHandle, 1.0f, true, true);
		ChangeSpawnTimer(_bjNormalArray, _bjNormalAnimClass, _bjMesh_2, 4, _bjNormalSpawnTimerHandle, 2.5f, true, true);
		bOnStage_7 = true;
	}

	if (_playTime >= 190.0f && !bOnStage_8)
	{
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		bOnStage_8 = true;
	}

	if (_playTime >= 210.0f && !bSrdSpawn_3)
	{
		SurroundSpawn(_gruxArray, _gruxAnimClass, _gruxMesh_2, 2, 50, 1000.0f);
		bSrdSpawn_3 = true;
	}

	if (_playTime >= 230.0f && !bOnStage_9)
	{
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 203);
		bOnStage_9 = true;
	}

	if (_playTime >= 250.0f && !bOnStage_10)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_1, 1, _gruxSpawnTimerHandle, 1.5f, true, true);
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_2, 2, _gruxSpawnTimerHandle, 1.0f, true, false);
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxMesh_3, 3, _gruxSpawnTimerHandle, 0.8f, true, false);

		ChangeSpawnTimer(_rampageNormalArray, _rampageNormalAnimClass, _rampageMesh_2, 2, _rampageNormalSpawnTimerHandle, 1.0f, true, true);
		ChangeSpawnTimer(_rampageNormalArray, _rampageNormalAnimClass, _rampageMesh_1, 3, _rampageNormalSpawnTimerHandle, 0.8f, true, false);

		ChangeSpawnTimer(_bjNormalArray, _bjNormalAnimClass, _bjMesh_2, 4, _bjNormalSpawnTimerHandle, 2.0f, true, true);

		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 202);

		bOnStage_10 = true;
	}

	if (_playTime >= 270.0f && !bSrdSpawn_4)
	{
		SurroundSpawn(_gruxArray, _gruxAnimClass, _gruxMesh_3, 3, 60, 1000.0f);
		bSrdSpawn_4 = true;
	}

	if (_playTime >= 280.0f && !bOnStage_11)
	{
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_1, 201);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_2, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_2, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_2, 202);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 203);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 203);
		SpawnMonster(_rampageBossArray, _rampageBossAnimClass, _rampageMesh_3, 203);
		bOnStage_11 = true;
	}

	if (_playTime >= 290.0f && !bOnStage_12)
	{
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		SpawnMonster(_bjBossArray, _bjBossAnimClass, _bjMesh_1, 204);
		bOnStage_12 = true;
	}

	if (_playTime >= 300.0f && !bOnEndStage)
	{
		PauseAllTimerHandle();

		bOnEndStage = true;
	}
}

bool ATFT_MonsterSpawnManager::IsAllCleared()
{
	for (auto m : _gruxArray)
	{
		if (m->bIsSpawned) return false;
	}

	for (auto m : _rampageBossArray)
	{
		if (m->bIsSpawned) return false;
	}

	for (auto m : _rampageNormalArray)
	{
		if (m->bIsSpawned) return false;
	}

	for (auto m : _bjBossArray)
	{
		if (m->bIsSpawned) return false;
	}

	for (auto m : _bjNormalArray)
	{
		if (m->bIsSpawned) return false;
	}

	return true;
}

void ATFT_MonsterSpawnManager::AddTimerHandleInArray(FTimerHandle* timerHandle)
{
	_timerHandles.Add(timerHandle);
}

void ATFT_MonsterSpawnManager::PauseAllTimerHandle()
{
	for (auto handle : _timerHandles)
	{
		GetWorldTimerManager().PauseTimer(*handle);
	}
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

void ATFT_MonsterSpawnManager::SetSpawnPos()
{
	for (int32 i = 0; i < 4; i++)
	{
		auto spawnPos = GetWorld()->SpawnActor(_spawnPosClass);

		if (spawnPos->IsValidLowLevel())
		{
			_spawnPositions.Add(spawnPos);

			switch (i)
			{
			case 0:
				spawnPos->SetActorLocation(_spawnPosVector1);
				break;

			case 1:
				spawnPos->SetActorLocation(_spawnPosVector2);
				break;

			case 2:
				spawnPos->SetActorLocation(_spawnPosVector3);
				break;

			case 3:
				spawnPos->SetActorLocation(_spawnPosVector4);
				break;

			default:
				break;
			}
		}
	}
}
