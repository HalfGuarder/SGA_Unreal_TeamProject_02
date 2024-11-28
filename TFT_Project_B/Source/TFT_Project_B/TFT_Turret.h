// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFT_Turret.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_Turret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFT_Turret();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void FindTarget();

	void Aiming();

	void Fire();

	void DeleteSelf();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* _btmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _midMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _topMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Target, meta = (AllowPrivateAccess = true))
	class ATFT_Monster* _target = nullptr;

	bool bTargetOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATFT_Projectile> _projectileClass;

	FTimerHandle _destroyTimerHandle;
	float timer = 0.0f;

public:
	bool bIsPreview = true;
};
