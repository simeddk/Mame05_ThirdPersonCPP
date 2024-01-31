#include "CAnimNotify_Backstepped.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Backstepped::GetNotifyName_Implementation() const
{
	return "Backstepped";
}

void UCAnimNotify_Backstepped::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Backstep();
}
