// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_BT_BOSS_FINDTARGET.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TFT_Boss_AIController.h"
#include "TFT_Creature.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "TFT_Player.h"

UTFT_BT_BOSS_FINDTARGET::UTFT_BT_BOSS_FINDTARGET()
{
	NodeName = TEXT("Find Target");
	Interval = 1.0f;
}

void UTFT_BT_BOSS_FINDTARGET::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    /*auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (currentPawn == nullptr)
        return;

    auto world = GetWorld();
    FVector center = currentPawn->GetActorLocation();
    float searchRadius = 2000.0f;

    if (world == nullptr)
        return;

    TArray<FOverlapResult> overlapResult;
    FCollisionQueryParams qparams(NAME_None, false, currentPawn);

    bool bResult = world->OverlapMultiByChannel
    (
        overlapResult,
        center,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel4,
        FCollisionShape::MakeSphere(searchRadius),
        qparams
    );
   
    if (bResult)
    {
        for (auto& result : overlapResult)
        {

            auto myPlayer = Cast<ATFT_Player>(result.GetActor());
            if (myPlayer != nullptr)
            {
                auto myPlayerController = myPlayer->GetController();
                if (myPlayerController != nullptr && myPlayerController->IsPlayerController())
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), myPlayer);

                    break;
                }
            }
        }

    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
    }*/
}
