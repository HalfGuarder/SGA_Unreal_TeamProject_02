// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_EffectManager.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TFT_FXs/TFT_Effect.h"

ATFT_EffectManager::ATFT_EffectManager()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	_rootComponent = CreateDefaultSubobject<USceneComponent>("RootCom");
	RootComponent = _rootComponent;

	CreateParticleClass(TEXT("Explosion"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/TFT_Rampage_Explosion.TFT_Rampage_Explosion_C'"));

	CreateParticleClass(TEXT("chainExplosion"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/TFT_Rampage_ChainExplosion.TFT_Rampage_ChainExplosion_C'"));

	//Player
	CreateParticleClass(TEXT("ShieldOn"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/TFT_Player_ShieldOnFX_BP.TFT_Player_ShieldOnFX_BP_C'"));
	CreateParticleClass(TEXT("Energy"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/energy.energy_C'"));
	CreateParticleClass(TEXT("TrunAttack"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/TFT_Player_TurnAttack.TFT_Player_TurnAttack_C'"));
	CreateParticleClass(TEXT("PosionAttack"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/TFT_Player_PosionEffect_BP.TFT_Player_PosionEffect_BP_C'"));

	CreateParticleClass(TEXT("ATTACKUP"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/AOE_ATTACK_BP.AOE_ATTACK_BP_C'"));
	CreateParticleClass(TEXT("SPEEDUP"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/AOE_SPEED_BP.AOE_SPEED_BP_C'"));

	CreateParticleClass(TEXT("FireMuzzle"), TEXT("/Script/Engine.Blueprint'/Game/Blueprints/VFX/TFT_Player_FireMuzzleFX_BP.TFT_Player_FireMuzzleFX_BP_C'"));
}


void ATFT_EffectManager::BeginPlay()
{
	Super::BeginPlay();
	CreateEffect();
}

void ATFT_EffectManager::CreateParticleClass(FString name, FString path)
{
	if (_classTable.Contains(name) == true)
	{
		UE_LOG(LogTemp, Error, TEXT("%s already exist in classTable"), *name);
		return;
	}

	ConstructorHelpers::FClassFinder<ATFT_Effect> effect(*path);
	if (effect.Succeeded())
	{
		_classTable.Add(name);
		_classTable[name] = effect.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("cant find : %s"), *path);
		return;
	}
}

void ATFT_EffectManager::CreateEffect()
{
	for (auto classPair : _classTable)
	{
		FString name = classPair.Key;

		_effectTable.Add(name);
		for (int32 i = 0; i < _poolCount; i++)
		{
			FString tempName = name + FString::FromInt(i);
			FActorSpawnParameters params;
			params.Name = FName(*tempName);
			auto effect = GetWorld()->SpawnActor<ATFT_Effect>(classPair.Value, FVector::ZeroVector, FRotator::ZeroRotator, params);

			effect->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			_effectTable[name].Add(effect);
		}
	}
}


void ATFT_EffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_EffectManager::Play(FString name, int32 effectType, FVector location, FRotator rotator)
{
	if (_effectTable.Contains(name) == false)
		return;

	auto findEffect = _effectTable[name].FindByPredicate(
		[](ATFT_Effect* effect)-> bool
		{
			if (effect->IsPlaying())
				return false;
			return true;
		});

	if (findEffect)
		(*findEffect)->Play(effectType, location, rotator);
}

