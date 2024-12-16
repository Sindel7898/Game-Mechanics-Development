// Fill out your copyright notice in the Description page of Project Settings.


#include "StreetLamp.h"

#include "CMP302Character.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStreetLamp::AStreetLamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATIC MESH"));
	LampMesh->SetupAttachment(RootComponent);


	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionSphere->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AStreetLamp::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AStreetLamp::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AStreetLamp::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AStreetLamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AStreetLamp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	 PlayerRef =  Cast<ACMP302Character>(OtherActor);
	 if (PlayerRef)
	 {
	 	GetWorldTimerManager().SetTimer(HealthTimer,this,&AStreetLamp::ReplenishPlayerHealth,0.5f,true);
	 	GetWorldTimerManager().SetTimer(EquipmentTimer,this,&AStreetLamp::ReplenishPlayerFlashAndGun,0.5f,true);
		 
	 }
	

}

void AStreetLamp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerRef = nullptr;
}

void AStreetLamp::ReplenishPlayerHealth()
{
	if (PlayerRef)
	{
		PlayerRef->Health++;

	}
	
	if (PlayerRef && PlayerRef->Health >= 100)
	{
		PlayerRef->Health = 100;
	}
	
}

void AStreetLamp::ReplenishPlayerFlashAndGun()
{
	if (PlayerRef)
	{
		PlayerRef->BatteryCount = 5;
		PlayerRef->GunAmmo = 30;
	}

}




