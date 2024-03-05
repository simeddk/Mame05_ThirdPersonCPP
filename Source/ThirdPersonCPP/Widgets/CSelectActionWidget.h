#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectActionWidget.generated.h"

UCLASS()
class THIRDPERSONCPP_API UCSelectActionWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

public:
	void Pressed(FString InName);

	FORCEINLINE class UCSelectActionItemWidget* GetItemWidget(FString InName) { return ItemWidgets[InName]; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<class UTexture2D*> Textures;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString Prefix = "Item";

	UPROPERTY(BlueprintReadOnly)
		TMap<FString, class UCSelectActionItemWidget*> ItemWidgets;

private:
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* ItemGrid;
};
