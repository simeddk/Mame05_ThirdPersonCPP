#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_MagicBall.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACDoAction_MagicBall : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void BeginSubAction() override;
	virtual void EndSubAction() override;

private:
	UPROPERTY(VisibleInstanceOnly)
		class UCAim* Aim;
};
