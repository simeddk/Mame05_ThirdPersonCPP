#include "CPlayerHealthWidget.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Characters/CPlayer.h"
#include "Components/CStatusComponent.h"

void UCPlayerHealthWidget::NativeConstruct()
{
	ACPlayer* player = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNull(player);

	CirclaGuageMaterial = CircleGuage->GetDynamicMaterial();
	
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(player);

	Update();

	Super::NativeConstruct();
}

void UCPlayerHealthWidget::Update_Implementation()
{
	CheckNull(StatusComp);

	CheckNull(RemainHealth);
	FString currentHealthStr = FString::FromInt((int32)StatusComp->GetCurrentHealth());
	RemainHealth->SetText(FText::FromString(currentHealthStr));

	CheckNull(CirclaGuageMaterial);
	CirclaGuageMaterial->SetScalarParameterValue("Ratio", StatusComp->GetCurrentHealth() / StatusComp->GetMaxHealth());

	PlayAnimationForward(HitEffect);
}
