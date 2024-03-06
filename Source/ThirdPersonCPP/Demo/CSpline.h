#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSpline.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSpline();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;
};
