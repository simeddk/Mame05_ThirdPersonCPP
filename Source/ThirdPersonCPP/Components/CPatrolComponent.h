#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPatrolComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsPathValid() { return Path != nullptr; }

	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);
	void UpdateNextIndex();

private:
	UPROPERTY(EditInstanceOnly, Category = "PatrolPath")
		class ACPatrolPath* Path;

	UPROPERTY(EditInstanceOnly, Category = "PatrolPath")
		int32 Index;

	UPROPERTY(EditInstanceOnly, Category = "PatrolPath")
		bool bReverse;

	UPROPERTY(EditInstanceOnly, Category = "PatrolPath")
		float AcceptanceRadius = 10.f;
};
