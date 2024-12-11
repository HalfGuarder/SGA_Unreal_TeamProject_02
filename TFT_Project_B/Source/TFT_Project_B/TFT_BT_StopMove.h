// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TFT_BT_StopMove.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_BT_StopMove : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTFT_BT_StopMove();


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
