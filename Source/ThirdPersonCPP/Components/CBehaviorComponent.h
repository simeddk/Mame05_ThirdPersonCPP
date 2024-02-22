#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChangedSignature, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }


public:
	UFUNCTION(BlueprintPure)
		bool IsWaitMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();

	class ACPlayer* GetPlayerKey();
	FVector GetLocationKey();

private:
	EBehaviorType GetType();
	void ChangeType(EBehaviorType InNewType);

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "BehaviorKey";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "PlayerKey";

	UPROPERTY(EditAnywhere)
		FName LocationKey = "LocationKey";

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChangedSignature OnBehaviorTypeChanged;


private:
	class UBlackboardComponent* Blackboard;
};
