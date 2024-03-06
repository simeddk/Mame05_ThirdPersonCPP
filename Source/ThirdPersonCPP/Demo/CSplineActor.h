#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "IInteractable.h"
#include "CSplineActor.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACSplineActor : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	ACSplineActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Interact(class ACharacter* InCharacter) override;

private:
	UFUNCTION()
		void StartTimeline(float Output);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

	UPROPERTY(EditAnywhere)
		class ACSpline* Spline;

private:
	FTimeline Timeline;
	class USplineComponent* SplineComp;
};
