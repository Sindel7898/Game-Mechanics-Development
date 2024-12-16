// Fill out your copyright notice in the Description page of Project Settings.


#include "CMP302/Private/FlashLight.h"

#include "Enemy.h"
#include "CMP302/Private/CMP302Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"

// Sets default values
AFlashLight::AFlashLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlashLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATIC MESH"));
	RootComponent = FlashLightMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionSphere->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlashLight::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AFlashLight::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AFlashLight::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFlashLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlashLight::InteractWithMe(ACMP302Character* Player)
{
	PlayerCharacter = Player;

	if(PlayerCharacter)
	{
		AttachToPlayerCamera();
	}
}

void AFlashLight::PerformLineTraces()
{
	FVector Start = GetActorLocation(); // Starting point of the trace
	//Start.Z+= 45;
	FVector ForwardVector = SpotLight->GetForwardVector(); // Forward direction
	FVector End = Start + (ForwardVector * TraceDistance); // End point of the trace

	FHitResult HitResult; // Structure to hold the result of the trace
	FCollisionQueryParams CollisionParams;

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		// If something was hit
		if (AActor* HitActor = HitResult.GetActor())
		{
			EnemyCharacter = Cast<AEnemy>(HitActor);
			
			if (EnemyCharacter)
			{
				GEngine->AddOnScreenDebugMessage(3,2,FColor::Green,FString::Printf(TEXT("enemy cant see")));

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



void AFlashLight::IncreaseLightIntensity()
{
	if (IsSpotLightVisibile == false)
	{
		SpotLight->SetVisibility(true);
		IsSpotLightVisibile = true;
	}

	// End intensity
	float EndIntensity = 50000.0f;
	
	// Increase intensity over time using a timer
	if (SpotLight->Intensity < EndIntensity && SpotLight->IsVisible())
	{
		float IntensityIncrement = 11000.0f;
		// Increment intensity
		SpotLight->SetIntensity(SpotLight->Intensity + IntensityIncrement);
		// Set a timer to call this function again after the interval
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AFlashLight::IncreaseLightIntensity);
	}
	

	if (SpotLight->Intensity  >  EndIntensity && SpotLight->IsVisible())
	{
		int TimeInterval = 4;
		GetWorld()->GetTimerManager().SetTimer(DecreaseIntensityTimeHandle,this,&AFlashLight::DecreaseLightIntensity,TimeInterval,false);
	}
	
	DisableFlashLight();
}


void AFlashLight::DecreaseLightIntensity()
{

	// End intensity
	float EndIntensity = 8000.0f;

	// Increase intensity over time using a timer
	if (SpotLight->Intensity >= EndIntensity   && SpotLight->IsVisible())
	{
		const float IntensityDecrement = 1000.0f;
		// Increment intensity
		SpotLight->SetIntensity(SpotLight->Intensity - IntensityDecrement);

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AFlashLight::DecreaseLightIntensity);
	}
	else
	{
		
		PlayerCharacter->BatteryCount -=1;
		PlayerCharacter->ShouldTimerTick = true;
	}
	
	
}
void AFlashLight::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AFlashLight::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
    
}

void AFlashLight::AttachToPlayerCamera()
{
	if (PlayerCharacter && PlayerCharacter->GetCameraBoom())
	{

		this->AttachToComponent(PlayerCharacter->GetFollowCamera(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorRelativeLocation(FVector(40.0f, 0.0f, -28.0f)); // Adjust as necessar
		SetActorRelativeRotation(FRotator(0.0f,271.0f,0.0f));
		FlashLightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FlashLightMesh->SetSimulatePhysics(false);
	}
}

void AFlashLight::DisableFlashLight()
{
	if (PlayerCharacter->BatteryCount == 0)
	{
		SpotLight->SetVisibility(false);
		IsSpotLightVisibile = false;
	}

	
}

