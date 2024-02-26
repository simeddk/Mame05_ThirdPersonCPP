#include "CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CPatrolComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCPatrolComponent* patrolComp = CHelpers::GetComponent<UCPatrolComponent>(controller->GetPawn());
	CheckNullResult(patrolComp, EBTNodeResult::Failed);

	FVector tempLocation;
	float tempRadius;
	CheckFalseResult(patrolComp->GetMoveTo(tempLocation, tempRadius), EBTNodeResult::Failed);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCPatrolComponent* patrolComp = CHelpers::GetComponent<UCPatrolComponent>(controller->GetPawn());
	CheckNull(patrolComp);

	FVector location;
	float acceptanceRadius;
	CheckFalse(patrolComp->GetMoveTo(location, acceptanceRadius));

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, acceptanceRadius, false);
	if (result == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		patrolComp->UpdateNextIndex();
		return;
	}
}
