// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TFT_Boss_AIController.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API ATFT_Boss_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATFT_Boss_AIController();

	virtual void OnPossess(APawn* InPawn)override;
	virtual void OnUnPossess() override;

	void RandMove();

	void ReStartBT();

private:

	UPROPERTY()
	class UBlackboardData* _bb;

	UPROPERTY()
	class UBehaviorTree* _bt;

	class UBehaviorTreeComponent* _btc;
};
