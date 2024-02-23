#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	if (!!Sight)
	{
		Sight->SightRadius = 600.f;
		Sight->LoseSightRadius = 800.f;
		Sight->PeripheralVisionAngleDegrees = 90.f;
		Sight->SetMaxAge(2.f);

		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;

		Perception->ConfigureSense(*Sight);
		Perception->SetDominantSense(Sight->GetSenseImplementation());
	}
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
	Behavior->SetBlackboard(Blackboard);
	
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebug)
	{
		FVector center = OwnerEnemy->GetActorLocation();
		DrawDebugSphere(GetWorld(), center, Sight->SightRadius, 30, FColor::Green);
		DrawDebugSphere(GetWorld(), center, BehaviorRange, 30, FColor::Red);
	}
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	
	ACPlayer* player = nullptr;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);

		if (!!player)
			break;
	}

	Blackboard->SetValueAsObject("PlayerKey", player);
}