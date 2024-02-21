#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/TimelineComponent.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void SetBodyColor(FLinearColor InColor);

	UFUNCTION()
		void ResetLogoColor();

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();
	void Dead();
	void End_Dead();

	UFUNCTION()
		void StartDissolve(float Output);

	UFUNCTION()
		void EndDissolve();

	//Scene Component
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

	//Actor Compoentn
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

private:
	UPROPERTY(EditAnywhere)
		float LaunchValue = 25.f;

	UPROPERTY(EditAnywhere)
		float DestroyPendingTime = 5.f;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* DissolveCurve;

	UPROPERTY(VisibleInstanceOnly)
		class UMaterialInstanceDynamic* DissolveMaterial;

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

	class ACharacter* Attacker;
	class AActor* Causer;
	float DamageValue;

	FTimeline DissolveTimeline;
};
