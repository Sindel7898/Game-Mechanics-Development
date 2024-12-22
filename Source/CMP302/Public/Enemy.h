// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class ULightDetection;
class USphereComponent;
class UBoxComponent;
class UCapsuleComponent;
class UBehaviorTree;
class UStaticMesh;


UCLASS()
class AEnemy : public ACharacter
{
	GENERATED_BODY()
protected:
	AEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnAnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,AActor* DamageCauser);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex);
	
	FORCEINLINE UCapsuleComponent* GetBodyCollider()    const { return GetCapsuleComponent();}
	
	void DisableWidget();
	void CheckIfEnemyIsDead();
	UFUNCTION(BlueprintCallable)
	float HealthPercentage();
	void DestroyEnemy();
	void TriggerExplosionEffect();
	void NegatePlayerHealth();

	UPROPERTY(EditAnywhere)
	class USphereComponent* HeadDamageCollider;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BodyDamageCollider;

	UPROPERTY(EditAnywhere)
	class USphereComponent* ColliderForPlayer;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DamageSound;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	UPROPERTY()
	FTimerHandle HealthTImer;
	UPROPERTY()
	FTimerHandle DamageWidgetTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Widget;
	
	float SeenTime = 0.0f;
	bool CanSee = true;
	float EnemyHealth = 100.0f;


	
	float DamageCount = 0;
	float DamageToSelf = 0;

	UPROPERTY()
	class ACMP302Character* Player;

public:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	float GetDamageToSelf();

	bool PlayerSeen = false;

	USphereComponent* GetHeadCollider(){ return  HeadDamageCollider;}
	UBoxComponent* GetBodyCollider(){ return  BodyDamageCollider;}
};
