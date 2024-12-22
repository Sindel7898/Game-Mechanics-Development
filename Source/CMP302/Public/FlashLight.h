// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "FlashLight.generated.h"

class AEnemy;
class USphereComponent;
class USpotLightComponent;
class ACMP302Character;

UCLASS()
class CMP302_API AFlashLight : public AActor,public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractWithMe(ACMP302Character* Player) override;
	void PerformLineTraces();
	void IncreaseLightIntensity();
	void DecreaseLightIntensity();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
	void AttachToPlayerCamera();
	void DisableFlashLight();


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FlashLightMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere)
	USpotLightComponent* SpotLight;

	UPROPERTY()
	ACMP302Character* PlayerCharacter = nullptr;
	
	UPROPERTY()
	class AEnemy* EnemyCharacter;
	
	FTimerHandle DecreaseIntensityTimeHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line Trace")
	float TraceDistance = 1000.0f;
     
	bool IsSpotLightVisibile = true;

};
