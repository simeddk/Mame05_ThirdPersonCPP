#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);

	//Combo Attack
	if (bCanCombo == true)
	{
		bCanCombo = false;
		bSucceed = true;
	}

	//First Attack
	CheckFalse(StateComp->IsIdleMode());
	StateComp->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	//Play Next Combo Montage
	CheckFalse(bSucceed);
	bSucceed = false;

	ComboCount++;
	ComboCount = FMath::Clamp(ComboCount, 0, Datas.Num() - 1);

	OwnerCharacter->StopAnimMontage();

	OwnerCharacter->PlayAnimMontage(Datas[ComboCount].AnimMontage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);
	Datas[ComboCount].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();
	ComboCount = 0;

	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACDoAction_Melee::Abort()
{
	Super::Abort();

	bSucceed = false;
	ComboCount = 0;
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InOverlappedComponent, InAttacker, InCauser, InOtherCharacter);

	//Register HittedCharacters Array for Multiple Hit
	int32 prevHittedCharactersNum = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);

	CheckFalse(prevHittedCharactersNum < HittedCharacters.Num());

	//Hack 02. TakeDamage가 HitStop보다 위에 있어야 한다.
	//Take Damage
	FDamageEvent damageEvent;
	InOtherCharacter->TakeDamage(Datas[ComboCount].Power, damageEvent, InAttacker->GetController(), InCauser);

	//Hack 03. UGameplayStatics::GetGlobalTimeDilation(GetWorld()) >= 1.f : 정상적인 게임 속도에서만 히트스탑을 걸어야 한다.(Slomo 중인 경우는 SKip)
	//Hit Stop
	float hitStop = Datas[ComboCount].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false && UGameplayStatics::GetGlobalTimeDilation(GetWorld()) >= 1.f)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalTimeDilation", hitStop * 2e-2f, false);
	}

	//Spawn Particle
	UParticleSystem* effect = Datas[ComboCount].Effect;
	if (!!effect)
	{
		FTransform trasnform = Datas[ComboCount].EffectTransform;
		trasnform.AddToTranslation(InOverlappedComponent->GetComponentLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, trasnform);
	}

	//CameraShake
	TSubclassOf<UCameraShake> shakeClass = Datas[ComboCount].ShakeClass;
	if (!!shakeClass)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shakeClass);
	}
}

void ACDoAction_Melee::OnAttachmentEndOverlap(UPrimitiveComponent* InOverlappedComponent, ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InOverlappedComponent, InAttacker, InCauser, InOtherCharacter);
}

void ACDoAction_Melee::ResetGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
