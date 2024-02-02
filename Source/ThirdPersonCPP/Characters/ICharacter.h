#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONCPP_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void SetBodyColor(FLinearColor InColor) = 0;
};
