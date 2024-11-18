// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Projectile.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

#include "TFT_Monster.h"

ATFT_Projectile::ATFT_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	_collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	_collider->InitCapsuleSize(10, 10);

	_meshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	_movementCom = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMove"));

	RootComponent = _collider;
	_meshCom->SetupAttachment(_collider);

	_movementCom->InitialSpeed = 2000.0f;
	_movementCom->MaxSpeed = 2000.0f;
	
}

void ATFT_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (_collider != nullptr)
	{
		_collider->OnComponentBeginOverlap.AddDynamic(this, &ATFT_Projectile::OnOverlapBegin);
	}
}

void ATFT_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator curRotation = GetActorRotation();
}

void ATFT_Projectile::FireInDirection(const FVector& ShootDirection)
{
	_movementCom->Velocity = ShootDirection * _movementCom->InitialSpeed;
	bIsFired = true;
}

void ATFT_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto enemy = Cast<ATFT_Monster>(OtherActor);

	if (enemy != nullptr)
	{
		FHitResult hitResult;
		FDamageEvent damageEvent;

		enemy->TakeDamage(10.0f, damageEvent, GetInstigatorController(), this);
		FVector hitPoint = GetActorLocation();

		DrawDebugSphere(GetWorld(), hitPoint, 10.0f, 10, FColor::Red, false, 0.5f);
	}
}

