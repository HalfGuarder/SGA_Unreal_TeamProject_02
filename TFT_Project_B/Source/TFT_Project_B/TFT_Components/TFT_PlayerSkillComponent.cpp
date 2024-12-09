// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Components/TFT_PlayerSkillComponent.h"

#include "TFT_Projectile.h"

#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"

UTFT_PlayerSkillComponent::UTFT_PlayerSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<ATFT_Projectile> pc
	(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Characters/Player/Weapons/TFT_Projectile_BP.TFT_Projectile_BP_C'"));
	if (pc.Succeeded())
	{
		_bulletClass = pc.Class;
	}

}

void UTFT_PlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateBullet(_bulletClass, _bulletArray, 20);

	_bulletPenetrationRate = 50.0f;
}

void UTFT_PlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTFT_PlayerSkillComponent::Fire(FVector start, FRotator rot, FVector fireDir)
{
	if (!_bulletArray.IsEmpty())
	{
		for (auto bullet : _bulletArray)
		{
			if (bullet->bIsFired) continue;

			bullet->Active();

			bullet->SetActorLocationAndRotation(start, rot);

			bullet->FireInDirection(fireDir, _bulletPenetrationRate);

			break;
		}
	}
}

void UTFT_PlayerSkillComponent::AttackHit(float damage, AController* controller)
{
	// FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, GetOwner());
	FVector playerLocation = GetOwner()->GetActorLocation();
	FVector playerFwdVector = GetOwner()->GetActorForwardVector();
	
	TArray<FHitResult> hitResults;

	bool bResult = GetWorld()->SweepMultiByChannel
	(
		hitResults,
		playerLocation,
		playerLocation + playerFwdVector * _swordAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeSphere(_swordAttackRadius),
		params
	);

	FVector vec = playerFwdVector * _swordAttackRange;
	FVector center = playerLocation + vec;
	FColor drawColor = FColor::Green;

	if (bResult && !hitResults.IsEmpty())
	{
		for (int32 i = 0; i < hitResults.Num(); i++)
		{
			if (hitResults[i].GetActor()->IsValidLowLevel())
			{
				drawColor = FColor::Red;
				FDamageEvent damageEvent;

				hitResults[i].GetActor()->TakeDamage(damage, damageEvent, controller, GetOwner());
				FVector hitPoint = hitResults[i].ImpactPoint;
			}
		}
	}

	/*bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		playerLocation,
		playerLocation + playerFwdVector * _swordAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeSphere(_swordAttackRadius),
		params
	);*/


	/*if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		FDamageEvent damageEvent;

		float actualDamage = hitResult.GetActor()->TakeDamage(damage, damageEvent, controller, GetOwner());
		FVector hitPoint = hitResult.ImpactPoint;

		if (actualDamage > 0)
		{
			ATFT_Creature* target = Cast<ATFT_Creature>(hitResult.GetActor());
			if (target != nullptr && !target->bIsOnState)
			{
				switch (_curAttackIndex)
				{
				case 1:
					target->SetState(StateType::Airborne);
					break;
				case 2:
					target->SetState(StateType::Stun);
					break;
				default:
					break;
				}
			}
		}
	}*/

	DrawDebugSphere(GetWorld(), center, _swordAttackRadius, 20, drawColor, false, 2.0f);
}

void UTFT_PlayerSkillComponent::CreateBullet(TSubclassOf<ATFT_Projectile> subclass, TArray<ATFT_Projectile*>& array, int32 num)
{
	for (int32 i = 0; i < num; i++)
	{
		auto projectile = GetWorld()->SpawnActor(subclass);

		ATFT_Projectile* bullet = Cast<ATFT_Projectile>(projectile);

		array.Add(bullet);
		bullet->DeActive();
	}
}

