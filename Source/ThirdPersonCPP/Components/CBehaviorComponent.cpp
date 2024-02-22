#include "CBehaviorComponent.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CPlayer.h"

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehaviorType::Patrol;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted;
}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehaviorType::Avoid;
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

void UCBehaviorComponent::ChangeType(EBehaviorType InNewType)
{
	EBehaviorType prev = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InNewType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(prev, InNewType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}

ACPlayer* UCBehaviorComponent::GetPlayerKey()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCBehaviorComponent::GetLocationKey()
{
	return Blackboard->GetValueAsVector(LocationKey);
}
