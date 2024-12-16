#include "TFT_Boss_AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ATFT_Boss_AIController::ATFT_Boss_AIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/AI/TFT_BOSS_BB.TFT_BOSS_BB'"));
	if (BB.Succeeded())
	{
		_bb = BB.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/TFT_BOSS_BT.TFT_BOSS_BT'"));
	if (BT.Succeeded())
	{
		_bt = BT.Object;
	}
}

void ATFT_Boss_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	_btc = Cast<UBehaviorTreeComponent>(BrainComponent);

	UBlackboardComponent* blackBoardComponent = Blackboard;
	if (UseBlackboard(_bb, blackBoardComponent))
	{
		if (RunBehaviorTree(_bt))
		{
			blackBoardComponent->SetValueAsVector(FName(TEXT("FixedPos")), InPawn->GetActorLocation());
		}
	}
}

void ATFT_Boss_AIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ATFT_Boss_AIController::RandMove()
{
	auto currentPawn = GetPawn();

	if (!currentPawn->IsValidLowLevel())
		return;

	UNavigationSystemV1* naviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (naviSystem == nullptr)
		return;

	FNavLocation randLocation;

	if (naviSystem->GetRandomPointInNavigableRadius(currentPawn->GetActorLocation(), 500.0f, randLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, randLocation);
	}
}

void ATFT_Boss_AIController::ReStartBT()
{
	if (_btc)
	{
		_btc->RestartTree();
	}
	
	// RunBehaviorTree(_bt);
}
