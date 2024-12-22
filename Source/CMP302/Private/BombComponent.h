// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombComponent.generated.h"

class UProjectileMovementComponent;

UCLASS()
class ABomb : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ABomb();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	///////////////////////////////
	void ApplyDamageToEnemies();
	void TriggerExplosionEffect();
	void RestPlayerPostProces();
	void ExplodeFunction();
	void ApplyDamageToPlayer();
    ///////////////////////////////
    
protected:
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	class URadialForceComponent* RadialForcecomponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShakeBase> ExplosionCameraShakeClass;

	UPROPERTY(EditAnywhere)
	class ACMP302Character* Player;
	
	UPROPERTY(EditAnywhere)
	float DamageRadius =  30;

	float  PlayerBloomIntensity = 15.0f; 
};
