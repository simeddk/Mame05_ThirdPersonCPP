#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "Widgets/CNameWidget.h"
#include "Widgets/CHealthWidget.h"
#include "Actions/CActionData.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateSceneComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateSceneComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	//Create ActorComponent
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	//Component Settings
	// -> Mesh
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* meshAsset;
	CHelpers::GetAsset<USkeletalMesh>(&meshAsset, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&animInstanceClass, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	CHelpers::GetAsset<UCurveFloat>(&DissolveCurve, "CurveFloat'/Game/Datas/Curve_Dissolve.Curve_Dissolve'");

	// -> Movement
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();

	// -> Widget
	TSubclassOf<UCNameWidget> nameWidgetClass;
	CHelpers::GetClass<UCNameWidget>(&nameWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameWidgetClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCHealthWidget> healthWidgetClass;
	CHelpers::GetClass<UCHealthWidget>(&healthWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy::BeginPlay()
{
	//Set Dynamic Material
	UMaterialInstanceConstant* bodyMaterialAsset, *logoMaterialAsset, *dissolveMaterialAsset;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&bodyMaterialAsset, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logoMaterialAsset, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_ChestLogo.M_UE4Man_ChestLogo'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&dissolveMaterialAsset, "MaterialInstanceConstant'/Game/Materials/MAT_Dissolve_Inst.MAT_Dissolve_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterialAsset, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterialAsset, this);
	DissolveMaterial = UMaterialInstanceDynamic::Create(dissolveMaterialAsset, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	//Bind StateType Changed Event
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	//Blueprint BeginPlay
	Super::BeginPlay();

	//Initialize Widget
	NameWidget->InitWidget();
	UCNameWidget* nameWidgetObject = Cast<UCNameWidget>(NameWidget->GetUserWidgetObject());
	if (!!nameWidgetObject)
	{
		const FString& controllerName = GetController()->GetName();
		const FString& characterName = GetName();

		nameWidgetObject->SetNames(controllerName, characterName);
	}

	HealthWidget->InitWidget();
	UCHealthWidget* healthWidgetObject = Cast<UCHealthWidget>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
	{
		float currentHP = Status->GetCurrentHealth();
		float maxHP = Status->GetMaxHealth();

		healthWidgetObject->Update(currentHP, maxHP);
	}

	//Bind Dissovle Timeline Event
	FOnTimelineFloat onProgress;
	onProgress.BindUFunction(this, "StartDissolve");
	DissolveTimeline.AddInterpFloat(DissolveCurve, onProgress);

	FOnTimelineEvent onFinish;
	onFinish.BindUFunction(this, "EndDissolve");
	DissolveTimeline.SetTimelineFinishedFunc(onFinish);

}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveTimeline.TickTimeline(DeltaTime);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	Causer = DamageCauser;

	Status->DecreaseHealth(DamageValue);

	if (Status->GetCurrentHealth() <= 0.f)
	{
		State->SetDeadMode();
		return DamageValue;
	}

	State->SetHittedMode();

	return DamageValue;
}

void ACEnemy::SetBodyColor(FLinearColor InColor)
{
	CheckTrue(State->IsDeadMode());

	if (State->IsHittedMode())
	{
		InColor *= 30.f;

		LogoMaterial->SetScalarParameterValue("bUseLogoLight", 1.f);
		LogoMaterial->SetVectorParameterValue("LogoColor", InColor);
		return;
	}

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEnemy::ResetLogoColor()
{
	LogoMaterial->SetScalarParameterValue("bUseLogoLight", 0.f);
	LogoMaterial->SetVectorParameterValue("LogoColor", FLinearColor::Black);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted:	Hitted();	break;
		case EStateType::Dead:		Dead();		break;
	}
}

void ACEnemy::Hitted()
{
	//Update Health Widget
	UCHealthWidget* healthWidgetObject = Cast<UCHealthWidget>(HealthWidget->GetUserWidgetObject());
	CheckNull(healthWidgetObject);

	healthWidgetObject->Update(Status->GetCurrentHealth(), Status->GetMaxHealth());

	//Play Hitted Montage
	Montages->PlayHitted();

	//Look At Attacker
	FVector start = GetActorLocation();
	FVector target = Attacker->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	//Launch Character
	FVector direction = (start - target).GetSafeNormal();
	LaunchCharacter(direction * DamageValue * LaunchValue, true, false);

	//Set Hitted Color
	SetBodyColor(FLinearColor::Red);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetLogoColor", 0.5f, false);
}

void ACEnemy::Dead()
{
	//CheckFalse(State->IsDeadMode());

	//Hidden Widgets
	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

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

	//Set Dissovle Material & Play Dissolve Timeline
	FLinearColor equipmentColor = Action->GetCurrentData()->EquipmentColor;
	DissolveMaterial->SetVectorParameterValue("BaseColor", equipmentColor);

	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); i++)
		GetMesh()->SetMaterial(i, DissolveMaterial);

	DissolveTimeline.PlayFromStart();
}

void ACEnemy::End_Dead()
{
	//Todo. Destroy All Owing Children

	Destroy();
}

void ACEnemy::StartDissolve(float Output)
{
	CheckNull(DissolveMaterial);

	DissolveMaterial->SetScalarParameterValue("Amount", Output);
}

void ACEnemy::EndDissolve()
{
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", DestroyPendingTime, false);
}


