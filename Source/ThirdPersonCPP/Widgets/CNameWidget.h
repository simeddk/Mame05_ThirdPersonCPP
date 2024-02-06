#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CNameWidget.generated.h"

UCLASS()
class THIRDPERSONCPP_API UCNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void SetNames(const FString& InControllerName, const FString& InCharacterName);
	void SetNames_Implementation(const FString& InControllerName, const FString& InCharacterName);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ControllerName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharacterName;
};
