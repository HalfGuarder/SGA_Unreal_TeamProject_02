// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

ATFT_AIController::ATFT_AIController()
{
}

void ATFT_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ATFT_AIController::OnUnPossess()
{
	Super::OnUnPossess();
}
