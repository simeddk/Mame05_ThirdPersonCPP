#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "Widgets/CNameWidget.h"
#include "Widgets/CHealthWidget.h"

ACEnemy::ACEnemy()
{
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
	UMaterialInstanceConstant* bodyMaterialAsset, *logoMaterialAsset;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&bodyMaterialAsset, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logoMaterialAsset, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_ChestLogo.M_UE4Man_ChestLogo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterialAsset, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterialAsset, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

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

	Action->SetUnaremdMode();
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	Causer = DamageCauser;

	//Todo. »¡°£ÁÙ~~~
	Status->DecreaseHealth(DamageValue);

	if (Status->GetCurrentHealth() <= 0.f)
	{
		State->SetDeadMode();
		return;
	}

	State->SetHittedMode();

	return DamageValue;
}

void ACEnemy::SetBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
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
}

void ACEnemy::Dead()
{
}


