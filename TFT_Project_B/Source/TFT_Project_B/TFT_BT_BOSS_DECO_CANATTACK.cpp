// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_BT_BOSS_DECO_CANATTACK.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "TFT_Boss_AIController.h"
#include "TFT_Creature.h"

UTFT_BT_BOSS_DECO_CANATTACK::UTFT_BT_BOSS_DECO_CANATTACK()
{
	NodeName = TEXT("CanAttack");
}

bool UTFT_BT_BOSS_DECO_CANATTACK::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (currentPawn == nullptr || target == nullptr)
	{
		return false;
	}
	float distance = target->GetDistanceTo(currentPawn);

	return distance < 400.0f;

}
