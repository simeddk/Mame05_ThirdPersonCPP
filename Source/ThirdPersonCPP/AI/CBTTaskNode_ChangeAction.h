#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CActionComponent.h"
#include "CBTTaskNode_ChangeAction.generated.h"

UCLASS()
class THIRDPERSONCPP_API UCBTTaskNode_ChangeAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_ChangeAction();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "ActionType")
		EActionType Type;
};
