#include "TFT_Managers/TFT_MonsterSpawnManager.h"

#include "TFT_BossMonster_Grux.h"
#include "TFT_AnimInstance_Grux.h"

ATFT_MonsterSpawnManager::ATFT_MonsterSpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<ATFT_BossMonster_Grux> mst
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Monster/BossMonster_Grux/TFT_BossMonster_Grux_BP.TFT_BossMonster_Grux_BP_C'"));
	if (mst.Succeeded())
	{
		_monsterClass = mst.Class;
	}
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> sm
	(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_2/Grux_Beetle_Red/Meshes/GruxBeetleRed.GruxBeetleRed'"));
	if (sm.Succeeded())
	{
		_monsterMesh = sm.Object;
	}

	ConstructorHelpers::FClassFinder<UTFT_AnimInstance_Grux> anim
	(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Monster/BossMonster_Grux/Animation/TFT_BossMonster_Grux_AnimInstance_BP.TFT_BossMonster_Grux_AnimInstance_BP_C'"));
	if (anim.Succeeded())
	{
		_monsterAnimClass = anim.Class;
	}
}

void ATFT_MonsterSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < 10; i++)
	{
		auto monster = GetWorld()->SpawnActor<ATFT_BossMonster_Grux>(_monsterClass, FVector(100.0f), FRotator::ZeroRotator);
		_monsters.Add(monster);
		monster->SetActorHiddenInGame(true);
		// monster->UnPossessed();
	}
	
}

void ATFT_MonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_spawnTimer += DeltaTime;

	if (_spawnTimer >= 3.0f)
	{
		SpawnMonster(_monsterMesh);
		_spawnTimer = 0;
	}

}

void ATFT_MonsterSpawnManager::SpawnMonster(TObjectPtr<USkeletalMesh> mesh)
{
	for (auto monster : _monsters)
	{
		if (monster->bIsSpawned) continue;

		monster->ChangeMesh(mesh);
		monster->GetMesh()->SetAnimInstanceClass(_monsterAnimClass);
		monster->SetAnimInstanceBind();
		monster->SetActorHiddenInGame(false);
		monster->bIsSpawned = true;

		return;
	}
}
