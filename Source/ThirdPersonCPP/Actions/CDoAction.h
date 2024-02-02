// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	//Todo. StateComp, StatusComp....

};
