// Fill out your copyright notice in the Description page of Project Settings.


#include "CustumPlayerController.h"

#include "Bird.h"
#include "CMP302Character.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

class ACMP302Character;

void ACustumPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<ACMP302Character> PlayerCLass = ACMP302Character::StaticClass();
	
	TArray<AActor*> FoundActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerCLass,FoundActors);

	for (AActor* actors : FoundActors)
	{
		PlayerRef = Cast<ACMP302Character>(actors);
		
		if (PlayerRef)
		{
			break;
		}
	}


	TSubclassOf<ABird> BirdClass = ABird::StaticClass();
	TArray<AActor*> FoundBirdActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),BirdClass,FoundBirdActors);

	for (AActor* actors : FoundBirdActors)
	{
		BirdRef = Cast<ABird>(actors);
		
		if (BirdRef)
		{
			break;
		}
	}
	
	Possess(PlayerRef);
}

void ACustumPlayerController::SwitchToBird()
{
	UnPossess();
	Possess(BirdRef);
	
}

void ACustumPlayerController::SwitchToPlayer()
{
	UnPossess();
	Possess(PlayerRef);
	
}
