#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class EMoveSpeedType : uint8
{
	Sneak, Wark, Sprint, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsCanMove() { return bCanMove; }

	FORCEINLINE float GetSneakSpeed() { return Speeds[(int32)EMoveSpeedType::Sneak]; }
	FORCEINLINE float GetWalkSpeed() { return Speeds[(int32)EMoveSpeedType::Wark]; }
	FORCEINLINE float GetSprintSpeed() { return Speeds[(int32)EMoveSpeedType::Sprint]; }

	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	void SetSpeed(EMoveSpeedType InType);

	void SetMove();
	void SetStop();

	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speeds[(int32)EMoveSpeedType::Max] = { 200.f, 400.f, 600.f };

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.f;

private:
	bool bCanMove = true;
	float CurrentHealth;
};
