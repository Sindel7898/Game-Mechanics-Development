// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class AEnemy;
class ACMP302Character;
class USphereComponent;
class UBoxComponent;
class UCapsuleComponent;

class UStaticMeshComponent;


UCLASS()
class AGun : public AActor,public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AttachToPlayerCamera();
	void PerformLineTrace ();
	virtual void InteractWithMe(ACMP302Character* Player) override;

	UPROPERTY()
	ACMP302Character* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line Trace")
	float TraceDistance = 1000.0f;

	

	
};
