#include "CSplineActor.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "CSpline.h"

ACSplineActor::ACSplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &Root, "Root");
	CHelpers::CreateSceneComponent(this, &Mesh, "Mesh", Root);
	CHelpers::CreateSceneComponent(this, &Camera, "Camera", Mesh);

	UStaticMesh* meshAsset;
	CHelpers::GetAsset<UStaticMesh>(&meshAsset, "StaticMesh'/Game/Demo/B787/Airplane_fuselage.Airplane_fuselage'");

	Mesh->SetStaticMesh(meshAsset);
	Mesh->SetRelativeScale3D(FVector(0.2f));
	Mesh->SetRelativeRotation(FRotator(0, -90, 0));

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Demo/Curve_Flying.Curve_Flying'");
}

void ACSplineActor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat onProgress;
	onProgress.BindUFunction(this, "StartTimeline");

	FOnTimelineEvent onFinish;
	onFinish.BindUFunction(this, "FinishTimeline");
	Timeline.SetTimelineFinishedFunc(onFinish);

	Timeline.AddInterpFloat(Curve, onProgress);
	Timeline.SetPlayRate(TimelineRate);
}

void ACSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ACSplineActor::Interact(ACharacter* InCharacter)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);
	controller->SetViewTarget(this);

	CheckNull(Spline);
	SplineComp = CHelpers::GetComponent<USplineComponent>(Spline);
	Timeline.PlayFromStart();
}


void ACSplineActor::StartTimeline(float Output)
{
	CheckNull(SplineComp);
	
	float length = SplineComp->GetSplineLength();

	FVector location = SplineComp->GetLocationAtDistanceAlongSpline(length * Output, ESplineCoordinateSpace::World);
	FRotator rotation = SplineComp->GetRotationAtDistanceAlongSpline(length * Output, ESplineCoordinateSpace::World);

	SetActorLocation(location);
	SetActorRotation(rotation);
}

void ACSplineActor::FinishTimeline()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(playerCharacter);
	controller->SetViewTargetWithBlend(playerCharacter, 2.f, EViewTargetBlendFunction::VTBlend_EaseOut, 2.f);
}