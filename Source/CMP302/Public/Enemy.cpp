// Fill out your copyright notice in the Description page of Project Settings.



// Sets default values
#include "Enemy.h"


#include "CMP302Character.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize DamageCollider as a sphere that detects collisions for damage
	DamageCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DAMAGE COLLIDER"));
	DamageCollider->SetupAttachment(GetRootComponent());

	// Bind overlap events to the appropriate functions
	DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);

	OnTakeAnyDamage.AddDynamic(this, &AEnemy::OnAnyDamageTaken);
	
	// Initialize widget component for displaying damage UI
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage Widget"));
	WidgetComponent->SetupAttachment(GetRootComponent());

	WidgetComponent->SetVisibility(false);

}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyHealth = 100.0f;
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

// Called when the enemy takes damage

void AEnemy::OnAnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	DamageToSelf = Damage;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),DamageSound,GetActorLocation());
	WidgetComponent->SetVisibility(true);
	// Set a timer to disable the widget after 2 seconds

	GetWorld()->GetTimerManager().SetTimer(DamageWidgetTimer, this, &AEnemy::DisableWidget, 2.f, false);

}
// Function to disable the damage widget after a delay

void AEnemy::DisableWidget()
{
	DamageToSelf = 0.0f;
	WidgetComponent->SetVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(DamageWidgetTimer);
}

// Function to handle damage taken by the enemy and decrease health
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageCount = Damage;
	EnemyHealth -= Damage * 1.2;
	CheckIfEnemyIsDead();

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}


// Check if the enemy is dead and trigger destruction if health is 0 or below
void AEnemy::CheckIfEnemyIsDead()
{
	if (EnemyHealth <= 0.0f)
	{
		FTimerHandle TimerHandle2;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &AEnemy::DestroyEnemy, 0.5f, false);
	}
}

// Function to destroy the enemy actor after a delay

void AEnemy::DestroyEnemy()
{

	TriggerExplosionEffect();
	Destroy();
}

// Function to get the behavior tree associated with the enemy
UBehaviorTree* AEnemy::GetBehaviorTree() const
{
	return Tree;
}

// Function to get the damage taken by the enemy
float AEnemy::GetDamageToSelf()
{
	return DamageToSelf;
}

// Function to return the health percentage of the enemy
float AEnemy::HealthPercentage()
{
	return EnemyHealth/ 100;
}



// Function to trigger explosion effect at the enemy's location
void AEnemy::TriggerExplosionEffect()
{
	if (ExplosionEffect)
	{
		FVector SpawnLocation = GetActorLocation(); // Or any other location
		FRotator SpawnRotation = FRotator::ZeroRotator;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, SpawnLocation, SpawnRotation);
	}
}

// Called when another actor begins to overlap with the enemy's damage collider
void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(9, 0.2f, FColor::Green, FString::Printf(TEXT(" OVERLAPPP")));

	// Check if the overlapping actor is the player
         	Player = Cast<ACMP302Character>(OtherActor);
            if (Player)
            {
            	// Start a timer to periodically damage the player
		          GetWorld()->GetTimerManager().SetTimer(HealthTImer, this, &AEnemy::NegatePlayerHealth, 0.3f, true);
            }
}

// Called when another actor ends overlap with the enemy's damage collider
void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
	Player = nullptr;
}

// Function to damage the player when overlapping
void AEnemy::NegatePlayerHealth()
{
	if (Player)
	{
		Player->Health-= 1 * 1.2;
	}
}