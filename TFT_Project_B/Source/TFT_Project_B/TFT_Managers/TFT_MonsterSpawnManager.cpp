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
	ConstructMonsterMesh(_gruxNormalMesh_1, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux'");
	ConstructMonsterMesh(_gruxNormalMesh_2, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_2/Grux_Beetle_Red/Meshes/GruxBeetleRed.GruxBeetleRed'");
	ConstructMonsterMesh(_gruxNormalMesh_3, "/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_2/Grux_Beetle_Molten/Meshes/GruxMolten.GruxMolten'");

	// Rampage_Boss
	ConstructMonsterSubclass(_rampageBossClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_DH/TFT_BossMonster_Rampage.TFT_BossMonster_Rampage_C'");
	ConstructMonsterAnimInst(_rampageBossAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_DH/Animation/TFT_AnimInstanceRampage_BP.TFT_AnimInstanceRampage_BP_C'");
	ConstructMonsterMesh(_rampageBossMesh, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");

	// Rampage_Normal
	ConstructMonsterSubclass(_rampageNormalClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/NormalMonster_Rampage/TFT_NormalMonster_Rampage_BP.TFT_NormalMonster_Rampage_BP_C'");
	ConstructMonsterAnimInst(_rampageNormalAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/NormalMonster_Rampage/Normal_Rampage_Animation/TFT_AnimInstance_NormalRampage_BP.TFT_AnimInstance_NormalRampage_BP_C'");
	ConstructMonsterMesh(_rampageNormalMesh, "/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'");

	// BJ_Boss
	ConstructMonsterSubclass(_bjBossClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_BJ/MyTFT_Boss_BJ.MyTFT_Boss_BJ_C'");
	ConstructMonsterAnimInst(_bjBossAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_BJ/Animations/TFT_BossAnimInstanceBJ_BP.TFT_BossAnimInstanceBJ_BP_C'");
	ConstructMonsterMesh(_bjBossMesh, "/Script/Engine.SkeletalMesh'/Game/Asset/SciFi_Soldiers/Mesh/Male/SK_SciFi_Soldier_Male_Skin4.SK_SciFi_Soldier_Male_Skin4'");

	// BJ_Normal
	ConstructMonsterSubclass(_bjNormalClass, "/Script/Engine.Blueprint'/Game/Blueprints/Monster/Normal_Monster_BJ/TFT_NormalMonster_BJ_BP.TFT_NormalMonster_BJ_BP_C'");
	ConstructMonsterAnimInst(_bjNormalAnimClass, "/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/Normal_Monster_BJ/NormalBJ_Animation/TFT_AnimInstance_NormalBJ_BP.TFT_AnimInstance_NormalBJ_BP_C'");
	ConstructMonsterMesh(_bjNormalMesh, "/Script/Engine.SkeletalMesh'/Game/Asset/SciFi_Soldiers/Mesh/Male/SK_SciFi_Soldier_Male_Skin1.SK_SciFi_Soldier_Male_Skin1'");
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

	CreateMonster(_gruxClass, _gruxArray, 100);
	CreateMonster(_rampageBossClass, _rampageBossArray, 10);
	CreateMonster(_rampageNormalClass, _rampageNormalArray, 50);
	CreateMonster(_bjBossClass, _bjBossArray, 10);
	CreateMonster(_bjNormalClass, _bjNormalArray, 50);

	// SetSpawnTimer(_gruxArray, _gruxSpawnTimerHandle, 3.0f, true);
	// SetSpawnTimer(_rampageBossArray,  _rampageBossSpawnTimerHandle, 10.0f, true);
}

void ATFT_MonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_playTime += DeltaTime;

	if (_playTime >= 1.0f && !bOnStage_1)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxNormalMesh_1, 2, _gruxSpawnTimerHandle, 3.0f, true);
		bOnStage_1 = true;
	}

	if (_playTime >= 20.0f && !bSrdSpawn_1)
	{
		_player = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
		SurroundSpawn(_gruxArray, _gruxAnimClass, _gruxNormalMesh_2, 4, 30, 900.0f);
		bSrdSpawn_1 = true;
	}

	if (_playTime >= 30.0f && !bOnStage_2)
	{
		ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxNormalMesh_3, 3, _gruxSpawnTimerHandle, 2.0f, true);
		ChangeSpawnTimer(_rampageBossArray, _rampageBossAnimClass, _rampageBossMesh, 4, _rampageBossSpawnTimerHandle, 1.0f, false);
		bOnStage_2 = true;
	}

	if (_playTime >= 40.0f && !bOnStage_3)
	{
		// ChangeSpawnTimer(_gruxArray, _gruxAnimClass, _gruxNormalMesh_1, 4, _gruxSpawnTimerHandle, 1.0f, true);
		ChangeSpawnTimer(_rampageBossArray, _rampageBossAnimClass, _rampageBossMesh, 204, _rampageBossSpawnTimerHandle, 5.0f, true);
		bOnStage_3 = true;
	}

	if (_playTime >= 50.0f && !bOnStage_4)
	{
		ChangeSpawnTimer(_rampageNormalArray, _rampageNormalAnimClass, _rampageNormalMesh, 203, _rampageNormalSpawnTimerHandle, 2.0f, true);
		bOnStage_4 = true;
	}

	if (_playTime >= 50.0f && !bOnStage_5)
	{
		ChangeSpawnTimer(_bjBossArray, _bjBossAnimClass, _bjBossMesh, 203, _bjBossSpawnTimerHandle, 5.0f, true);
		bOnStage_5 = true;
	}

	if (_playTime >= 60.0f && !bOnStage_6)
	{
		ChangeSpawnTimer(_bjNormalArray, _bjNormalAnimClass, _bjNormalMesh, 4, _bjNormalSpawnTimerHandle, 2.0f, true);
		bOnStage_6 = true;
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
