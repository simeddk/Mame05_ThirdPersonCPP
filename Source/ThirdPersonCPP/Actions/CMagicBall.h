#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CMagicBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMagicBallOverlapSignatrue, FHitResult, InHitResult);

UCLASS()
class THIRDPERSONCPP_API ACMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ACMagicBall();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetData(const FDoActionData& InData) { Data = InData; }

private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FMagicBallOverlapSignatrue OnMagicBallOverlap;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

private:
	FDoActionData Data;

};
