#include "CDoor.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"

ACDoor::ACDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &Root, "Root");
	CHelpers::CreateSceneComponent(this, &DoorFrame, "DoorFrame", Root);
	CHelpers::CreateSceneComponent(this, &Door, "Door", DoorFrame);

	UStaticMesh* doorFrameMeshAsset, *doorMeshAsset;

	CHelpers::GetAsset<UStaticMesh>(&doorFrameMeshAsset, "StaticMesh'/Game/DoorMesh/Props/SM_DoorFrame.SM_DoorFrame'");
	CHelpers::GetAsset<UStaticMesh>(&doorMeshAsset, "StaticMesh'/Game/DoorMesh/Props/SM_Door.SM_Door'");
	
	DoorFrame->SetStaticMesh(doorFrameMeshAsset);
	Door->SetStaticMesh(doorMeshAsset);
	Door->SetRelativeLocation(FVector(0, 45, 0));
}

void ACDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

