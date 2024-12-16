// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StreetLamp.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class USpotLightComponent;
class ACMP302Character;

UCLASS()
class AStreetLamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStreetLamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*  LampMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere)
	USpotLightComponent* SpotLight;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex);


	void ReplenishPlayerHealth();
	void ReplenishPlayerFlashAndGun();

	 ACMP302Character*  PlayerRef;

	FTimerHandle HealthTimer;
	FTimerHandle EquipmentTimer;

};
