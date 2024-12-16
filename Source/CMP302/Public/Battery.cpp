// Fill out your copyright notice in the Description page of Project Settings.


#include "CMP302/Private/Battery.h"

#include "CMP302/Private/CMP302Character.h"
#include "Components/SphereComponent.h"

// Sets default values
ABattery::ABattery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionSphere->SetupAttachment(RootComponent);
	
	
}

// Called when the game starts or when spawned
void ABattery::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABattery::InteractWithMe(ACMP302Character* Player)
{

	Player->BatteryCount++;

	Destroy();
}

// Called every frame
void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

