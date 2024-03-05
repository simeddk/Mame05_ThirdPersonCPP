#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectActionItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FImageButtonPressedSignature);

UCLASS()
class THIRDPERSONCPP_API UCSelectActionItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetTextureToImageButton(class UTexture2D* InTexture);

private:
	UFUNCTION(BlueprintCallable)
		void OnPressed();
	
	UFUNCTION(BlueprintCallable)
		void OnHover();

	UFUNCTION(BlueprintCallable)
		void OnUnhover();

private:
	class UCSelectActionWidget* GetParentWidget();

public:
	UPROPERTY(BlueprintAssignable)
		FImageButtonPressedSignature OnImageButtonPressed;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ImageButton;
};
