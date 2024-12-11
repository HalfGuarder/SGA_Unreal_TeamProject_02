// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_BT_StopMove.h"
#include "TFT_Boss_AIController.h"

UTFT_BT_StopMove::UTFT_BT_StopMove()
{
	NodeName = TEXT("Stop Movement");
}

EBTNodeResult::Type UTFT_BT_StopMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        AIController->StopMovement(); // AI 이동 중지
        return EBTNodeResult::Succeeded; // 작업 성공
    }

    return EBTNodeResult::Failed; // 작업 실패
}
