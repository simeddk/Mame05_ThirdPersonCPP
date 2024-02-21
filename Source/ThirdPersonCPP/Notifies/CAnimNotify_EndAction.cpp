#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData_Spawned.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData_Spawned* actionData = actionComp->GetCurrentData();
	CheckNull(actionData);

	ACDoAction* doAction = actionData->GetDoAction();
	CheckNull(doAction);
	
	doAction->End_DoAction();
}