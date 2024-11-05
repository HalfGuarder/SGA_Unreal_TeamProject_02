#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TFT_BT_TASK_BOSS_ATTACK.generated.h"

/**
 * 
 */
UCLASS()
class TFT_PROJECT_B_API UTFT_BT_TASK_BOSS_ATTACK : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTFT_BT_TASK_BOSS_ATTACK();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

private:
	bool _isAttacking = false;
};
