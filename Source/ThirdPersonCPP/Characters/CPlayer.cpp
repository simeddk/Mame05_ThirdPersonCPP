#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Blueprint/UserWidget.h"
#include "Components/PostProcessComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Widgets/CPlayerHealthWidget.h"
#include "Widgets/CSelectActionWidget.h"
#include "Widgets/CSelectActionItemWidget.h"
#include "Demo/IInteractable.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create Scene Component
	CHelpers::CreateSceneComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateSceneComponent(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateSceneComponent(this, &PostProcess, "PostProcess", GetRootComponent());

	//Create Actor Component
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");

	//Component Settings
	// -> MeshComp
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	USkeletalMesh* meshAsset;
	CHelpers::GetAsset<USkeletalMesh>(&meshAsset, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&animInstanceClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	// -> SpringArmComp
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bUsePawnControlRotation = true;

	// -> MovementComp
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	// -> PostProcess
	PostProcess->Settings.bOverride_VignetteIntensity = false;
	PostProcess->Settings.VignetteIntensity = 2.f;

	PostProcess->Settings.bOverride_DepthOfFieldFocalDistance = false;
	PostProcess->Settings.DepthOfFieldFocalDistance = 2.f;

	//Get Widget ClassRef
	CHelpers::GetClass<UCPlayerHealthWidget>(&HealthWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_PlayerHealth.WB_PlayerHealth_C'");
	CHelpers::GetClass<UCSelectActionWidget>(&SelectActionWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_SelectAction.WB_SelectAction_C'");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Set Dynamic Materials
	UMaterialInstanceConstant* bodyMaterialAsset;
	UMaterialInstanceConstant* logoMaterialAsset;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&bodyMaterialAsset, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logoMaterialAsset, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_ChestLogo.M_UE4Man_ChestLogo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterialAsset, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterialAsset, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	//Bind StateType Chagned Event
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	//Bind Hitted Event
	OnHittedEvent.AddDynamic(this, &ACPlayer::End_Roll);
	OnHittedEvent.AddDynamic(this, &ACPlayer::End_Backstep);

	Action->SetUnaremdMode();

	//Create Widget
	HealthWidget = Cast<UCPlayerHealthWidget>(CreateWidget(GetController<APlayerController>(), HealthWidgetClass));
	CheckNull(HealthWidget);
	HealthWidget->AddToViewport();

	SelectActionWidget = Cast<UCSelectActionWidget>(CreateWidget(GetController<APlayerController>(), SelectActionWidgetClass));
	CheckNull(SelectActionWidget);
	SelectActionWidget->AddToViewport();
	SelectActionWidget->SetVisibility(ESlateVisibility::Hidden);

	SelectActionWidget->GetItemWidget("Item1")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnFist);
	SelectActionWidget->GetItemWidget("Item2")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnOneHand);
	SelectActionWidget->GetItemWidget("Item3")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnTwoHand);
	SelectActionWidget->GetItemWidget("Item4")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnMagicBall);
	SelectActionWidget->GetItemWidget("Item5")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnWarp);
	SelectActionWidget->GetItemWidget("Item6")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnTornado);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("Tornado", EInputEvent::IE_Pressed, this, &ACPlayer::OnTornado);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoSubAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffDoSubAction);

	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSelectAction);
	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Released, this, &ACPlayer::OffSelectAction);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteract);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	Causer = DamageCauser;

	//Action->AbortedByDamaged();

	Status->DecreaseHealth(DamageValue);
	HealthWidget->Update();

	if (Status->GetCurrentHealth() <= 0.f)
	{
		State->SetDeadMode();
		return DamageValue;
	}

	State->SetHittedMode();

	Action->AbortedByDamaged();

	return DamageValue;
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckTrue(FMath::IsNearlyZero(InAxis));
	CheckFalse(Status->IsCanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckTrue(FMath::IsNearlyZero(InAxis));
	CheckFalse(Status->IsCanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	float rate = Option->GetZoomSpeed() * InAxis * GetWorld()->GetDeltaSeconds();

	SpringArm->TargetArmLength += rate;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, Option->GetZoomMin(), Option->GetZoomMax());
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->IsCanMove());
	
	if (InputComponent->GetAxisValue("MoveForward") < 0.f)
	{
		State->SetBackstepMode();
		return;
	}

	State->SetRollMode();
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(State->IsIdleMode());

	Action->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarpMode();
}

void ACPlayer::OnTornado()
{
	CheckFalse(State->IsIdleMode());

	Action->SetTornadoMode();
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::OnDoSubAction()
{
	Action->DoSubAction(true);
}

void ACPlayer::OffDoSubAction()
{
	Action->DoSubAction(false);
}

void ACPlayer::OnSelectAction()
{
	CheckFalse(State->IsIdleMode());
	CheckNull(SelectActionWidget);

	GetController<APlayerController>()->bShowMouseCursor = true;
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

	SelectActionWidget->SetVisibility(ESlateVisibility::Visible);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffSelectAction()
{
	CheckNull(SelectActionWidget);

	GetController<APlayerController>()->bShowMouseCursor = false;
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	SelectActionWidget->SetVisibility(ESlateVisibility::Hidden);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ACPlayer::OnInteract()
{
	CheckFalse(State->IsIdleMode());

	FVector start = GetActorLocation();
	FVector end = start + Camera->GetForwardVector() * 150.f;

	TArray<AActor*> ignores;
	ignores.Add(this); 

	for (const auto& child : Children)
	{
		ignores.Add(child);
	}

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		true,
		ignores,
		DebugInteractType,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		2.f
	);

	CheckFalse(hitResult.bBlockingHit);
	CLog::Log("LineTraced Actor is " + hitResult.GetActor()->GetName());

	IIInteractable* interactable = Cast<IIInteractable>(hitResult.GetActor());
	CheckNull(interactable);

	if ((Camera->GetForwardVector() | GetActorForwardVector()) > 0)
		interactable->Interact(this);
}

void ACPlayer::Hitted(EStateType InPrevType)
{
	//Todo. It makes idle state force
	/*if (OnHittedEvent.IsBound())
		OnHittedEvent.Broadcast();*/

	Status->SetStop();
	Montages->PlayHitted();
}

void ACPlayer::Dead()
{
	//Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->GlobalAnimRateScale = 0.f;

	//Add Force
	FVector start = GetActorLocation();
	FVector target = Attacker->GetActorLocation();
	FVector direction = (start - target).GetSafeNormal();
	FVector force = direction * LaunchValue * DamageValue;
	GetMesh()->AddForceAtLocation(force, start);

	//Off All Collisions
	Action->OffAllCollisions();

	//Dead Effect
	PostProcess->Settings.bOverride_VignetteIntensity = true;
	PostProcess->Settings.bOverride_DepthOfFieldFocalDistance = true;

	DisableInput(GetController<APlayerController>());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);

	//End Dead Timer
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 2.f, false);
}

void ACPlayer::End_Dead()
{
	APlayerController* controller = GetController<APlayerController>();
	CheckNull(controller);

	controller->ConsoleCommand("RestartLevel");
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();

	FVector target;
	if (GetVelocity().IsNearlyZero())
	{
		target = start + Camera->GetForwardVector().GetSafeNormal2D();
	}
	else
	{
		target = start + GetVelocity().GetSafeNormal2D();
	}

	FRotator forceRotation = UKismetMathLibrary::FindLookAtRotation(start, target);
	SetActorRotation(FRotator(0, forceRotation.Yaw, 0));

	Montages->PlayRoll();
}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackstep();
}

void ACPlayer::End_Roll()
{
	State->SetIdleMode();

	if (!!Action->GetCurrentDataAsset()
		&& Action->GetCurrentDataAsset()->GetEquipmentData().bLookForward == true)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}

void ACPlayer::End_Backstep()
{
	State->SetIdleMode();

	if ( !!Action->GetCurrentDataAsset()
		 && Action->GetCurrentDataAsset()->GetEquipmentData().bLookForward == false)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void ACPlayer::SetBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll:		Begin_Roll();				break;
		case EStateType::Backstep:	Begin_Backstep();			break;
		case EStateType::Hitted:	Hitted(InPrevType);			break;
		case EStateType::Dead:		Dead();						break;
	}
}

