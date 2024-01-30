#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCOptionComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

	FORCEINLINE float GetZoomSpeed() { return ZoomSpeed; }
	FORCEINLINE float GetZoomMin() { return ZoomRange.X; }
	FORCEINLINE float GetZoomMax() { return ZoomRange.Y; }
		
private:
	UPROPERTY(EditAnywhere, Category = "Mouse")
		float HorizontalLookRate = 45.f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		float VerticalLookRate = 45.f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		float ZoomSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		FVector2D ZoomRange = FVector2D(100, 1000);
};
