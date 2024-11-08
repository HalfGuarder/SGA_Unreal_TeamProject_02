// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_BossMonster_CG.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_BossMonster_CG : public ATFT_Monster
{
	GENERATED_BODY()
	
public:
	ATFT_BossMonster_CG();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = true))
	class UTFT_AnimInstance_Boss_CG* _animInstance_Boss;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* HelmetMeshComponent;
};
