// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustumPlayerController.generated.h"

class ABird;
class ACMP302Character;
class UInputMappingContext;

UCLASS()
class CMP302_API ACustumPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void SwitchToBird();
	void SwitchToPlayer();

	UPROPERTY()
	UInputMappingContext* MappingContext;

	UPROPERTY()
	ACMP302Character* PlayerRef;

	UPROPERTY()
	ABird* BirdRef;
};
