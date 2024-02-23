#include "CBTTaskNode_DoAction.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_DoAction::UCBTTaskNode_DoAction()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_DoAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(controller->GetPawn());
	CheckNullResult(actionComp, EBTNodeResult::Failed);

	RunningTime = 0.f;
	actionComp->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_DoAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(controller->GetPawn());
	CheckNull(stateComp);

	RunningTime += DeltaSeconds;

	if (RunningTime >= Delay && stateComp->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}
