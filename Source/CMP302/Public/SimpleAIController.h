// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SimpleAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

UCLASS()
class ASimpleAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASimpleAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};
