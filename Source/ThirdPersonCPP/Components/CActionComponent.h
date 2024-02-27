#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Fist, OneHand, TwoHand, Warp, MagicBall, Tornado, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChangedSignature, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCActionData_Spawned* GetCurrentData() { return Datas[(int32)Type]; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCActionData* GetCurrentDataAsset() { return DataAssets[(int32)Type]; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsMagicBallMode() { return Type == EActionType::MagicBall; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTornadoMode() { return Type == EActionType::Tornado; }

public:
	UFUNCTION(BlueprintCallable) void SetUnaremdMode();
	UFUNCTION(BlueprintCallable) void SetFistMode();
	UFUNCTION(BlueprintCallable) void SetOneHandMode();
	UFUNCTION(BlueprintCallable) void SetTwoHandMode();
	UFUNCTION(BlueprintCallable) void SetWarpMode();
	UFUNCTION(BlueprintCallable) void SetMagicBallMode();
	UFUNCTION(BlueprintCallable) void SetTornadoMode();

	void OffAllCollisions();
	void End_Dead();
	void AbortedByDamaged();

private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

public:
	void DoAction();
	void DoSubAction(bool bEnable);

public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChangedSignature OnActionTypeChanged;

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DataAssets[(int32)EActionType::Max];

private:
	EActionType Type;

	UPROPERTY()
		class UCActionData_Spawned* Datas[(int32)EActionType::Max];
		
};
