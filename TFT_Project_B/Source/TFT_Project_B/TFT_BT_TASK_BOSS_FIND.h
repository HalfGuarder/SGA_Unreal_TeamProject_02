#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TFT_BT_TASK_BOSS_FIND.generated.h"

UCLASS()
class TFT_PROJECT_B_API UTFT_BT_TASK_BOSS_FIND : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTFT_BT_TASK_BOSS_FIND();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
};
