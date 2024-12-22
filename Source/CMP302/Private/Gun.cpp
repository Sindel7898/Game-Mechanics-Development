// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "CMP302Character.h"
#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = GunMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionSphere->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AGun::InteractWithMe(ACMP302Character* Player)
{
	PlayerCharacter = Player;

	if(PlayerCharacter)
	{
		AttachToPlayerCamera();
	}
}



void AGun::AttachToPlayerCamera()
{
	if (PlayerCharacter && PlayerCharacter->GetCameraBoom())
	{

		this->AttachToComponent(PlayerCharacter->GetFollowCamera(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorRelativeLocation(FVector(57.0f, 30.0f, -51.0f)); 
		GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GunMesh->SetSimulatePhysics(false);
	}
}



void AGun::PerformLineTrace() 
{
	FVector Start = GetActorLocation(); // Starting point of the trace
	Start.Z+= 45;
	FVector ForwardVector = GetActorForwardVector(); // Forward direction
	FVector End = Start + (ForwardVector * TraceDistance); // End point of the trace

	FHitResult HitResult; // Structure to hold the result of the trace
	FCollisionQueryParams CollisionParams;

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		// If something was hit
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (AEnemy* EnemyCharacter = Cast<AEnemy>(HitActor))
			{
			 	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			 	
			    if (HitComponent == EnemyCharacter->GetHeadCollider())
			    {
			    	EnemyCharacter->TakeDamage(70,FDamageEvent(),nullptr,this);
			    	GEngine->AddOnScreenDebugMessage(3,2,FColor::Green,FString::Printf(TEXT("enemy taken damage on Head")));
			    }
			 	else if (HitComponent == EnemyCharacter->GetBodyCollider())
			 	{
			 		EnemyCharacter->TakeDamage(40,FDamageEvent(),nullptr,this);
			 		GEngine->AddOnScreenDebugMessage(3,2,FColor::Green,FString::Printf(TEXT("enemy taken damage on body")));
			 	}
			}
		}

		// Draw a debug line in the world for visual feedback
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Green, false, 1.0f);
	}
	else
	{
		// Draw a debug line in the world if nothing was hit
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, 0, 1.0f);
	}
}
