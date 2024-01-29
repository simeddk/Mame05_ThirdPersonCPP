#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* ownerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(ownerCharacter);

	Speed = ownerCharacter->GetVelocity().Size2D();

	FRotator controlRotation = FRotator(0, ownerCharacter->GetControlRotation().Yaw, 0);
	Direction = CalculateDirection(ownerCharacter->GetVelocity(), controlRotation);
	
}