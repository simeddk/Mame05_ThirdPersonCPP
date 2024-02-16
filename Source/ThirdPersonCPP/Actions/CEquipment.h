#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegateSignature);

UCLASS()
class THIRDPERSONCPP_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEquipment();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetData(FEquipmentData& InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor& InColor) { Color = InColor; }
	FORCEINLINE const bool* IsEquippedThis() { return &bEquippedThis; }

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegateSignature OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegateSignature OnUnequipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* StatusComp;

private:
	FEquipmentData Data;
	FLinearColor Color;

	bool bEquippedThis;
};
