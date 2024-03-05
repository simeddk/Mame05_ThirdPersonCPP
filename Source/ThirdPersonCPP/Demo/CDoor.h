#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDoor.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Door;

};
