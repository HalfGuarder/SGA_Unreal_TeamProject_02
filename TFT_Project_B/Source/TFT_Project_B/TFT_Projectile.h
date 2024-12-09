// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_Projectile.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFT_Projectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection, float penetrationRate);

	void Active();
	void DeActive();

	FTimerHandle _lifeTimerHandle;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class UCapsuleComponent* _collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* _meshCom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* _movementCom;

public:
	bool bIsFired = false;
	float _penetrationRate = 0.0f;
};
