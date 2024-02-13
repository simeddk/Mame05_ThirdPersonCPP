#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class THIRDPERSONCPP_API UCAim : public UObject
{
	GENERATED_BODY()

public:
	UCAim();

	void BeginPlay(class ACharacter* InOwnerCharacter);
	void Tick(float DeltaTime);
	
public:
	FORCEINLINE bool IsCanAim() { return SpringArm != nullptr && Camera != nullptr; }

	void On();
	void Off();

private:
	UFUNCTION()
		void Zooming(float Output);
	
private:
	class ACharacter* OwnerCharacter;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bZooming;

	FTimeline AimTimeline;
	class UCurveFloat* Curve;

	class ACHUD* HUD;
};
