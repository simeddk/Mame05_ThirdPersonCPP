#include "CAnimNotify_Idle.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Characters/CPlayer.h"

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

	UCStatusComponent* statusComp = CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	CheckNull(statusComp);

	stateComp->SetIdleMode();
	statusComp->SetMove();

	//Todo. Not Good....
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);
	if (player->OnHittedEvent.IsBound())
		player->OnHittedEvent.Broadcast();
}