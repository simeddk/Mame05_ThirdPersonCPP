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
	FRotator leftRotation;
	Trace(LeftSocketName, leftDistance, leftRotation);

	float rightDistance;
	FRotator rightRotation;
	Trace(RightSocketName, rightDistance, rightRotation);

	float airDistance = FMath::Min(leftDistance, rightDistance);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, leftDistance - airDistance, DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - airDistance), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, airDistance, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.f;
	OutRotation = FRotator::ZeroRotator;

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
	OutDistance = CorrectionValue + undergroundLegnth - Extension;

	FVector normal = hitResult.ImpactNormal;

	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);

	pitch = FMath::Clamp(pitch, -30.f, +30.f);
	roll = FMath::Clamp(roll, -15.f, +15.f);

	OutRotation = FRotator(pitch, 0.f, roll);
}

