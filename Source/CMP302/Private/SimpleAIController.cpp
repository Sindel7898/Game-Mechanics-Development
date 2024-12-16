// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAIController.h"

#include "Bird.h"
#include "BehaviorTree/BehaviorTree.h"


ASimpleAIController::ASimpleAIController(FObjectInitializer const& ObjectInitializer)
{
}

void ASimpleAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABird* BirdRef = Cast<ABird>(InPawn))
	{
		UBlackboardComponent* BlackBoard;
		UseBlackboard(BirdRef->GetBehaviorTree()->GetBlackboardAsset(),BlackBoard);
		RunBehaviorTree(BirdRef->GetBehaviorTree());
	}
}


