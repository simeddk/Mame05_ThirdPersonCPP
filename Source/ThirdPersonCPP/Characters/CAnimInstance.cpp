#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* ownerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(ownerCharacter);

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(ownerCharacter);
	CheckNull(actionComp);

	actionComp->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* ownerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(ownerCharacter);

	Speed = ownerCharacter->GetVelocity().Size2D();

	FRotator controlRotation = FRotator(0, ownerCharacter->GetControlRotation().Yaw, 0);
	Direction = CalculateDirection(ownerCharacter->GetVelocity(), controlRotation);

	UCFeetComponent* feetComp = CHelpers::GetComponent<UCFeetComponent>(ownerCharacter);
	CheckNull(feetComp);

	FeetData = feetComp->GetData();
	//Todo. ABP
	
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}
