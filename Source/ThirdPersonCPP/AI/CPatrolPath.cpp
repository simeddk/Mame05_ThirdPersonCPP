#include "CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	CHelpers::CreateSceneComponent(this, &Root, "Root");
	CHelpers::CreateSceneComponent(this, &Spline, "Spline", Root);
	CHelpers::CreateSceneComponent(this, &Text, "Text", Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30));
	Spline->ScaleVisualizationWidth = 30.f;
	Spline->bShouldVisualizeScale = true;
	Spline->EditorUnselectedSplineSegmentColor = FLinearColor::Red;

	Text->SetRelativeLocation(FVector(0, 0, 100));
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetTextRenderColor(FColor::Red);
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Text->SetText(GetActorLabel());
	Spline->SetClosedLoop(bLoop);

	
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
	Text->SetVisibility(false);

}

