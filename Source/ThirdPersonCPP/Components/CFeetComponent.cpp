#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;
	Trace(LeftSocketName, leftDistance);

	float rightDistance;
	Trace(RightSocketName, rightDistance);

	float airDistance = FMath::Min(leftDistance, rightDistance);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, leftDistance - airDistance, DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, rightDistance - airDistance, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance)
{
	OutDistance = 0.f;

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FVector start = FVector(socketLocation.X, socketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	float endZ = start.Z - CapsuleHalfHeight - Extension;
	FVector end = FVector(start.X, start.Y, endZ);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		ignores,
		DrawDebugType,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		2.f
	);
	
	CheckFalse(hitResult.bBlockingHit);
	
	float undergroundLegnth = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = CorrectionValue + undergroundLegnth - Extension; //????

}

