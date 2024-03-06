#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Abort() override;
	
	virtual void OnAttachmentBeginOverlap(class UPrimitiveComponent* InOverlappedComponent,  class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class UPrimitiveComponent* InOverlappedComponent, class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;

public:
	FORCEINLINE void OnCombo() { bCanCombo = true; }
	FORCEINLINE void OffCombo() { bCanCombo = false; }
	FORCEINLINE void ClearHittedCharacters() { HittedCharacters.Empty(); }

private:
	UFUNCTION()
		void ResetGlobalTimeDilation();

private:
	int32 ComboCount;

	bool bCanCombo;
	bool bSucceed;

	TArray<class ACharacter*> HittedCharacters;
};
