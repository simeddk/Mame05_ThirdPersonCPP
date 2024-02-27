#include "CBTService_Magic.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Magic::UCBTService_Magic()
{
	NodeName = "Root_Magic";
}

void UCBTService_Magic::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Get Components
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behaviorComp);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	//Check Dead
	if (stateComp->IsDeadMode())
	{
		behaviorComp->SetWaitMode();
		return;
	}

	//Set Behavior Hitted
	if (stateComp->IsHittedMode())
	{
		behaviorComp->SetHittedMode();
		return;
	}

	//Try to Get Target Player
	ACPlayer* player = behaviorComp->GetPlayerKey();

	//No Perceived Player
	if (player == nullptr)
	{
		behaviorComp->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::LastFocusPriority);
		return;
	}

	//Perceived Player
	controller->SetFocus(player);
	float distance = enemy->GetDistanceTo(player);

	//In BehaviorRange
	if (distance < controller->GetBehaviorRange())
	{
		behaviorComp->SetAvoidMode();
		return;
	}

	//In Sight
	if (distance < controller->GetSightRadius())
	{
		behaviorComp->SetActionMode();
		return;
	}
}
