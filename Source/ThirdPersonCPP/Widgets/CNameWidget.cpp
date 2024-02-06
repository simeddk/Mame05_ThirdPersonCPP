#include "CNameWidget.h"
#include "Global.h"
#include "Components/TextBlock.h"

void UCNameWidget::SetNames_Implementation(const FString& InControllerName, const FString& InCharacterName)
{
	ControllerName->SetText(FText::FromString(InControllerName));
	CharacterName->SetText(FText::FromString(InCharacterName));
}
