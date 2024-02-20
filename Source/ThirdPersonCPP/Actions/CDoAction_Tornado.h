#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Tornado.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACDoAction_Tornado : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

private:
	UFUNCTION()
		void TickDamage();

	UFUNCTION()
		void Finish();

	virtual void OnAttachmentBeginOverlap(class UPrimitiveComponent* InOverlappedComponent, class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class UPrimitiveComponent* InOverlappedComponent, class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly)
		float DamageTime = 0.1f;

	UPROPERTY(EditDefaultsOnly)
		float ActiveTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
		float AroundSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly)
		float Radius = 150.f;

	UPROPERTY()
		class UParticleSystemComponent* Particle;

private:
	class UBoxComponent* Box;
	bool bActivating;

	TArray<class ACharacter*> HittedCharacters;
	float CurrentYaw;

};
