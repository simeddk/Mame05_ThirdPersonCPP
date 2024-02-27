#include "CEnvQueryContext_Player.h"
#include "Global.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CBehaviorComponent.h"

void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController* controller = Cast<ACAIController>(enemy->GetController());
	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACPlayer* player = behaviorComp->GetPlayerKey();

	if (!!player)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, player);
		return;
	}

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, enemy);
}