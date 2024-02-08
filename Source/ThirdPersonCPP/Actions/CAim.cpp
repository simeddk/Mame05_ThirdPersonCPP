#include "CAim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

UCAim::UCAim()
{

}

void UCAim::BeginPlay(class ACharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);

	//Todo... 1881818
}

void UCAim::Tick(float DeltaTime)
{

}