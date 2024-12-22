// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"

#include "CMP302Character.h"
#include "Enemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SimpleAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABird::ABird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up flying Details
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->MaxFlySpeed = 600.f; 
	GetCharacterMovement()->BrakingDecelerationFlying = 2000.f;
	GetCharacterMovement()->AirControl = 1.f; 
	GetCharacterMovement()->MaxAcceleration =  2048.f;
	////////////////////////////////////////////////////////////////////////
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();

	//Get Controller
	AIController = Cast<ASimpleAIController>(this->GetController<ASimpleAIController>());
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckifBirdLookingAtEnemy();

	if (Controller != nullptr && Cast<APlayerController>(Controller))
	{
		AddMovementInput(GetActorForwardVector());
	}
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdDefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(BirdLookAction, ETriggerEvent::Triggered, this, &ABird::Look);
		EnhancedInputComponent->BindAction(AccendButton  , ETriggerEvent::Triggered, this, &ABird::Ascend);
		EnhancedInputComponent->BindAction(DescendButton , ETriggerEvent::Triggered, this, &ABird::Descend);
		EnhancedInputComponent->BindAction(SwitchButton  , ETriggerEvent::Completed, this, &ABird::SwitchToPlayer);
	}

}

//movde down 
void ABird::Ascend(const FInputActionValue& Value)
{
	AddMovementInput(FVector::UpVector, Value.Get<float>());
}
//move up
void ABird::Descend(const FInputActionValue& Value)
{
	AddMovementInput(-FVector::UpVector, Value.Get<float>());
}


void ABird::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y * 0.5f);
	}
}

void ABird::SwitchToPlayer()
{
	TSubclassOf<ACMP302Character> PlayerClass = ACMP302Character::StaticClass();
	TArray<AActor*> FoundPlayerActor;
	
	//find other character
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerClass,FoundPlayerActor);

	for (AActor* actors : FoundPlayerActor)
	{
		PlayerRef = Cast<ACMP302Character>(actors);
		
		if (PlayerRef)
		{
			if (GetController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Switched to Player."));
				
				if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
				{
					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
					{
						Subsystem->ClearAllMappings(); 
						Subsystem->AddMappingContext(PlayerRef->GetMappingContext(), 0);
						GetWorld()->GetTimerManager().ClearTimer(RandomMovementTimer);
						GetController()->Possess(PlayerRef);
						
						if (AIController)
						{
							//possess
							AIController->Possess(this);
						}
					}
				}
				
				break; 
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Controller is null. Cannot possess Player."));
			}
		}
	}
}

void ABird::CheckifBirdLookingAtEnemy()
{
	TSubclassOf<AEnemy> EnemyClass = AEnemy::StaticClass();
	TArray<AActor*> FoundEnemyActor;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),EnemyClass,FoundEnemyActor);

	for (AActor* actors : FoundEnemyActor)
	{
	     AEnemy* EnemyRef = Cast<AEnemy>(actors);
		
		if (EnemyRef)
		{    //calculate where the actor is looking
			FVector BirdForward = GetActorForwardVector().GetSafeNormal();
			FVector DirectionToEnemy = (EnemyRef->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			float DotProduct = FVector::DotProduct(BirdForward, DirectionToEnemy);

			//spawn out materials
			if (DotProduct >0.8)
			{
				EnemyRef->GetMesh()->SetMaterial(0,EmmisiveMaterial);
				EnemyRef->GetMesh()->SetMaterial(1,EmmisiveMaterial);
			}
			else
			{
				if (EnemyRef->GetMesh()->GetMaterial(0) != EnemyDefultMat && EnemyRef->GetMesh()->GetMaterial(1) != EnemyDefultMat2 )
				{
					EnemyRef->GetMesh()->SetMaterial(0,EnemyDefultMat);
					EnemyRef->GetMesh()->SetMaterial(1,EnemyDefultMat2);
				}
			
			}
		}
	}
	
}