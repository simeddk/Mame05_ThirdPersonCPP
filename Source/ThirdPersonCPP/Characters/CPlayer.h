#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHittedEventSignature);

UCLASS()
class THIRDPERSONCPP_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//Axis Event
private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);

	//Action Event
private:
	void OnEvade();
	void OnWalk();
	void OffWalk();

	void OnFist();
	void OnOneHand();
	void OnTwoHand();
	void OnMagicBall();
	void OnWarp();
	void OnTornado();

	void OnDoAction();

	void OnDoSubAction();
	void OffDoSubAction();

	void Hitted(EStateType InPrevType);
	void Dead();

	UFUNCTION()
		void End_Dead();

	//Common Montage Play
private:
	void Begin_Roll();
	void Begin_Backstep();

public:
	UFUNCTION()
		void End_Roll();

	UFUNCTION()
		void End_Backstep();

public:
	virtual void SetBodyColor(FLinearColor InColor) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	//Scene Component
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UPostProcessComponent* PostProcess;

	//Actor Component
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Dead")
		float LaunchValue = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCPlayerHealthWidget> HealthWidgetClass;

public:
	UPROPERTY(BlueprintAssignable)
		FHittedEventSignature OnHittedEvent;


private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

	float DamageValue;
	class ACharacter* Attacker;
	class AActor* Causer;

	class UCPlayerHealthWidget* HealthWidget;
};
