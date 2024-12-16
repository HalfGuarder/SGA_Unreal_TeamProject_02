// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TFT_BT_BOSS_FINDTARGET.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_BT_BOSS_FINDTARGET : public UBTService
{
	GENERATED_BODY()
	
public:
	UTFT_BT_BOSS_FINDTARGET();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
};
