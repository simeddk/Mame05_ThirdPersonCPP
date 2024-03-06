#include "CSpline.h"
#include "Global.h"
#include "Components/SplineComponent.h"

ACSpline::ACSpline()
{
	CHelpers::CreateSceneComponent(this, &Spline, "Spline");
}

void ACSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

