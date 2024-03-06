#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

UINTERFACE(MinimalAPI)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONCPP_API IIInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact(class ACharacter* InCharacter) = 0;
};
