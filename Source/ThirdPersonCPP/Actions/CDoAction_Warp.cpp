#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CAttachment.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	//Attahcment->WarpPreview
	for (AActor* child : OwnerCharacter->Children)
	{
		if (child->IsA<ACAttachment>() && child->GetActorLabel().Contains("Warp"))
		{
			Preview = CHelpers::GetComponent<UStaticMeshComponent>(child);
			break;
		}
	}
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Preview->SetVisibility(false);

	CheckFalse(*bEquippedThis);

	FVector location;
	FRotator rotation;
	if (GetCursorLocationAndRotation(location, rotation))
	{
		Preview->SetVisibility(true);
		Preview->SetWorldLocation(location + Preview->GetUpVector() * 125);
		Preview->SetWorldRotation(rotation);
	}
}

void ACDoAction_Warp::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComp->IsIdleMode());

	FRotator temp;
	CheckFalse(GetCursorLocationAndRotation(Location, temp));

	StateComp->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	FTransform transform = Datas[0].EffectTransform;
	UGameplayStatics::SpawnEmitterAttached
	(
		Datas[0].Effect,
		OwnerCharacter->GetMesh(),
		"",
		transform.GetLocation(),
		FRotator(transform.GetRotation()),
		transform.GetScale3D()
	);
}

void ACDoAction_Warp::End_DoAction()
{
	Super::End_DoAction();

	Location.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	OwnerCharacter->SetActorLocation(Location);

	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	
	FHitResult hitResult;
	if (playerController->GetHitResultUnderCursorForObjects(objectTypes, true, hitResult))
	{
		OutLocation = hitResult.Location;

		FVector normal = hitResult.ImpactNormal;
		float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);
		float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);

		OutRotation = FRotator(pitch, 0, roll);

		return true;
	}

	return false;
}
