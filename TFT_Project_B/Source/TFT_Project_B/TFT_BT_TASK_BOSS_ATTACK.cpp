#include "TFT_BT_TASK_BOSS_ATTACK.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TFT_Boss_AIController.h"
#include "TFT_Creature.h"
#include "TFT_Boss_BJ.h"
#include "TFT_Player.h"
#include "TFT_Monster.h"

UTFT_BT_TASK_BOSS_ATTACK::UTFT_BT_TASK_BOSS_ATTACK()
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UTFT_BT_TASK_BOSS_ATTACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto character = Cast<ATFT_Monster>(OwnerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;

	character->Attack_AI();
	_isAttacking = true;

	character->_attackEndedDelegate.AddLambda(
		[this]()->void
		{
			this->_isAttacking = false;
		});

	return result;
}

void UTFT_BT_TASK_BOSS_ATTACK::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	

    if (_isAttacking == false)
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
