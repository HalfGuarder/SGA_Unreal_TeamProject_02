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

	static ConstructorHelpers::FClassFinder<AActor> sPos
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Spawn/TFT_SpawnPos_BP.TFT_SpawnPos_BP_C'"));
	if (sPos.Succeeded())
	{
		_spawnPosClass = sPos.Class;
	}

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

	CreateMonster(_gruxClass, _gruxArray, _gruxAnimClass, _gruxMesh, 100);
	CreateMonster(_rampageBossClass, _rampageBossArray, _rampageBossAnimClass, _rampageBossMesh, 10);

	// SetSpawnTimer(_gruxArray, _gruxSpawnTimerHandle, 3.0f, true);
	// SetSpawnTimer(_rampageBossArray,  _rampageBossSpawnTimerHandle, 10.0f, true);
}

void ATFT_MonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_playTime += DeltaTime;

	if (_playTime >= 1.0f && !bOnStage_1)
	{
		ChangeSpawnTimer(_gruxArray, 2, _gruxSpawnTimerHandle, 3.0f, true);
		bOnStage_1 = true;
	}

	if (_playTime >= 20.0f && !bSrdSpawn_1)
	{
		_player = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
		SurroundSpawn(_gruxArray, 4, 30, 900.0f);
		bSrdSpawn_1 = true;
	}

	if (_playTime >= 30.0f && !bOnStage_2)
	{
		ChangeSpawnTimer(_gruxArray, 3, _gruxSpawnTimerHandle, 2.0f, true);
		ChangeSpawnTimer(_rampageBossArray, 4, _rampageBossSpawnTimerHandle, 1.0f, false);
		bOnStage_2 = true;
	}

	if (_playTime >= 60.0f && !bOnStage_3)
	{
		ChangeSpawnTimer(_gruxArray, 4, _gruxSpawnTimerHandle, 1.0f, true);
		ChangeSpawnTimer(_rampageBossArray, 5, _rampageBossSpawnTimerHandle, 5.0f, true);
		bOnStage_3 = true;
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
