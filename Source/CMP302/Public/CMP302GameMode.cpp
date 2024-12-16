// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302/Private/CMP302GameMode.h"

#include "CustumPlayerController.h"
#include "CMP302/Private/CMP302Character.h"
#include "UObject/ConstructorHelpers.h"

ACMP302GameMode::ACMP302GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));

	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		//DefaultPawnClass = ACMP302Character::StaticClass();
	}

	//PlayerControllerClass = ACustumPlayerController::StaticClass();
}
