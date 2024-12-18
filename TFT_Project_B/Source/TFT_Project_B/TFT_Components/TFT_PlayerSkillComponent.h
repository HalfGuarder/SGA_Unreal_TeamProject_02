// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFT_PlayerSkillComponent.generated.h"

class ATFT_Projectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFT_PROJECT_B_API UTFT_PlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTFT_PlayerSkillComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Fire(FVector start, FRotator rot, FVector fireDir);

	void AttackHit(float damage, class AController* controller);

private:
	void CreateBullet(TSubclassOf<ATFT_Projectile> subclass, TArray<ATFT_Projectile*>& array, int32 num);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sword, meta = (AllowPrivateAccess = "true"))
	float _swordAttackRange = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sword, meta = (AllowPrivateAccess = "true"))
	float _swordAttackRadius = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rifle, meta = (AllowPrivateAccess = "true"))
	float _bulletPenetrationRate = 0.0f;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rifle, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATFT_Projectile> _bulletClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rifle, meta = (AllowPrivateAccess = "true"))
	TArray<ATFT_Projectile*> _bulletArray;
};
