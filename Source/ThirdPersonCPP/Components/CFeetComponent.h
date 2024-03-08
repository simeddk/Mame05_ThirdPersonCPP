#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeetData")
		FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeetData")
		FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeetData")
		FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeetData")
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeetData")
		FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
 	FORCEINLINE const FFeetData& GetData() { return Data; }

private:
	void Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName LeftSocketName = "Foot_L";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName RightSocketName = "Foot_R";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float Extension = 55.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float CorrectionValue = 5.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float InterpSpeed = 17.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;

private:
	FFeetData Data;
		
	class ACharacter* OwnerCharacter;
	float CapsuleHalfHeight;
};
