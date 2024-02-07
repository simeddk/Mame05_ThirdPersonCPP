#include "CAnimNotify_Idle.h"
#include "Global.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_Idle::GetNotifyName_Implementation() const
{
	return "Idle";
}

void UCAnimNotify_Idle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());
	
	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(stateComp);

	stateComp->SetIdleMode();
}