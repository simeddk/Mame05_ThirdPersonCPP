#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerHealthWidget.generated.h"

UCLASS()
class THIRDPERSONCPP_API UCPlayerHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent)
		void Update();
	void Update_Implementation();

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* CircleGuage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RemainHealth;

	UPROPERTY(meta = (BindWidgetAnim))
		class UWidgetAnimation* HitEffect;

private:
	class UCStatusComponent* StatusComp;
	class UMaterialInstanceDynamic* CirclaGuageMaterial;
};
