// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TFT_BT_BOSS_DECO_CANATTACK.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_BT_BOSS_DECO_CANATTACK : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UTFT_BT_BOSS_DECO_CANATTACK();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
