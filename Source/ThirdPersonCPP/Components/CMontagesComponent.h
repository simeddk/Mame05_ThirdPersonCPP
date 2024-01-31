#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CStateComponent.h"
#include "CMontagesComponent.generated.h"

//-----------------------------------------------------------------------------
//Struct Table Row Based
//-----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove;
};

//-----------------------------------------------------------------------------
//Class MontagesComp
//-----------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayRoll();
	void PlayBackstep();

private:
	void PlayAnimMontage(EStateType InStateType);

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;
	

private:
	FMontageData* Datas[(int32)EStateType::Max];
};
