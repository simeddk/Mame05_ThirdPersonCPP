#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() {};
	virtual void Begin_DoAction() {};
	virtual void End_DoAction() {};

	virtual void BeginSubAction() {};
	virtual void EndSubAction() {};

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void SetDatas(TArray<FDoActionData>& InDatas) { Datas = InDatas; }
	FORCEINLINE void SetEquippedThis(const bool* InEquipped) { bEquippedThis = InEquipped; }

public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class UPrimitiveComponent* InOverlappedComponent, class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) {};
	
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class UPrimitiveComponent* InOverlappedComponent, class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) {};

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* StatusComp;

protected:
	TArray<FDoActionData> Datas;
	
	const bool* bEquippedThis;
};
