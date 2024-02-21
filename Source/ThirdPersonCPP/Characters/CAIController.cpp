#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent(this, &Blackboard, "Blackboard");
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	//Todo. 설명은 내일.. 포폴 얘기도..
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
