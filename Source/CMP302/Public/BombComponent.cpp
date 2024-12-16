// Fill out your copyright notice in the Description page of Project Settings.


#include "BombComponent.h"

#include "CMP302Character.h"
#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values for this component's properties
ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;  // This enables the tick function

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetRelativeLocation(FVector::ZeroVector);
	MeshComponent->SetRelativeRotation(FRotator::ZeroRotator);

	// Create Default subobjects for components
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->InitialSpeed = 100.0f;
	ProjectileMovementComponent->MaxSpeed = 300.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;

	
	RadialForcecomponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	RadialForcecomponent->SetupAttachment(MeshComponent);
	RadialForcecomponent->Radius = DamageRadius;
	RadialForcecomponent->ImpulseStrength = 500.0f;
	RadialForcecomponent->bImpulseVelChange = true;
	RadialForcecomponent->bAutoActivate = false; 
	RadialForcecomponent->bIgnoreOwningActor = true;
}


// Called when the game starts
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABomb::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Get all player characters in the world
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACMP302Character::StaticClass(), PlayerActors);

	for (AActor* FoundActor : PlayerActors)
	{
		// Cast the actor to a player character
		Player = Cast<ACMP302Character>(FoundActor);

		if (Player)
		{
			// Apply an impulse in the direction of the player's forward vector
			MeshComponent->AddImpulse(Player->GetFollowCamera()->GetForwardVector() * 100 * MeshComponent->GetMass() ,NAME_None,true);

			// Set a timer to trigger the explosion after 5 seconds
			FTimerHandle ExplodeTimer;
			GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &ABomb::ExplodeFunction, 5.0f, false);
		}
	}
	
}


void ABomb::ExplodeFunction()
{
	// Apply damage to the player and enemies within the explosion radius
	ApplyDamageToPlayer();
	ApplyDamageToEnemies();

	// If the RadialForce component is valid, trigger the impulse
	if (RadialForcecomponent)
	{
		// Apply radial impulse to affected objects
		RadialForcecomponent->FireImpulse();
	}
	
	TriggerExplosionEffect();
}


void ABomb::ApplyDamageToPlayer()
{
	// If the player exists, proceed with damage calculation
		if (Player)
		{
			// Calculate the distance between the bomb and the player
			float Distance = FVector::Dist(MeshComponent->GetComponentLocation(), Player->GetActorLocation());
			
			// If the player is within the damage radius
			if (Distance <= DamageRadius)
			{
				// Calculate the damage based on the distance from the explosion
				float DamageScale = 1.0f - (Distance / DamageRadius);
				float DamageToApply = 50 * DamageScale;
				
				// Apply bloom effect on the player's camera
				Player->GetFollowCamera()->PostProcessSettings.bOverride_BloomIntensity = true;
				Player->GetFollowCamera()->PostProcessSettings.BloomIntensity =  PlayerBloomIntensity;

				// Subtract health based on calculated damage
				Player->Health -= DamageToApply;
			}
		}
	
}


void ABomb::ApplyDamageToEnemies()
{
	// Get all enemies in the world
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyActors);

	// Loop through each enemy found
	for (AActor* FoundActor : EnemyActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(FoundActor);
		
		if (Enemy)
		{
			// Calculate the distance between the bomb and the enemy
			float Distance = FVector::Dist(MeshComponent->GetComponentLocation(), Enemy->GetActorLocation());
			
			if (Distance <= DamageRadius)
			{
			
				
				// Apply the calculated damage to the enemy
				Enemy->TakeDamage(80,FDamageEvent(),nullptr,this);

			}
		}
	}
}


void ABomb::TriggerExplosionEffect()
{
	if (ExplosionEffect)
	{
		// Get the location and rotation for the explosion effect
		FVector SpawnLocation = MeshComponent->GetComponentLocation(); 
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// Spawn the explosion effect at the specified location and rotation
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, SpawnLocation, SpawnRotation);

		// Play the explosion sound if it is set
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, MeshComponent->GetComponentLocation());
			
		}
		

		   // Play camera shake for the explosion
			UGameplayStatics::PlayWorldCameraShake(
				GetWorld(),
				ExplosionCameraShakeClass,
				GetActorLocation(),
				0.0f,
				600.0f,
				1.0f,
				true );


		// Destroy the bomb's components after explosion
		MeshComponent->DestroyComponent();
		RadialForcecomponent->DestroyComponent();
		ProjectileMovementComponent->DestroyComponent();

		//Reduce the players bloom amount every 0.02 seconds
		FTimerHandle ExplodeTimer;
		GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &ABomb::RestPlayerPostProces, 0.02f, true);
	}
}

void ABomb::RestPlayerPostProces()
{
	//Bloom deduction
	PlayerBloomIntensity--;
	Player->GetFollowCamera()->PostProcessSettings.BloomIntensity =  PlayerBloomIntensity;

	//Destroy Actor
	if (PlayerBloomIntensity == 0.0f)
	{
		Destroy();
	}
}