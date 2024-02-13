#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACHUD();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

public:
	FORCEINLINE void EnableCrossHair() { bVisbleCrossHair = true; }
	FORCEINLINE void DisableCrossHair() { bVisbleCrossHair = false; }

private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* CrossHairTexture;

private:
	class UCStateComponent* StateComp;
	UEnum* StateTypeAsUEnum;

	bool bVisbleCrossHair;

};
