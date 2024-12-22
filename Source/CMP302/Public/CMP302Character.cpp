// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302/Private/CMP302Character.h"

#include "Bird.h"
#include "BombComponent.h"
#include "Enemy.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gun.h"
#include "CMP302/Private/FlashLight.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/KismetMathLibrary.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACMP302Character

ACMP302Character::ACMP302Character()
{
	/*WallDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("WALL COLLIDER"));
	WallDetector->SetupAttachment(GetRootComponent());*/
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	PrimaryActorTick.bCanEverTick = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 5.5;
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetFieldOfView(110);
	JumpMaxCount = 2;
	
}

void ACMP302Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	holddeltatime = DeltaTime;

	if(ShouldTimerTick)
	{
		DecreaseTime(DeltaTime);
	}

	if (bAdjustingFOV)
	{
		float CurrentFOV = FollowCamera->FieldOfView;
		float NewFOV = FMath::FInterpTo(CurrentFOV, 130.0f, DeltaTime, 4.0f); // Smoothly interpolate
		FollowCamera->SetFieldOfView(NewFOV);

		if (FMath::IsNearlyEqual(NewFOV, 130.0f, 0.01f))
		{
			bAdjustingFOV = false;
		}
		
	}


	if (bRestFOV)
	{
		float CurrentFOV = FollowCamera->FieldOfView;
		float NewFOV = FMath::FInterpTo(CurrentFOV, 110.0f, DeltaTime, 4.0f); // Smoothly interpolate
		FollowCamera->SetFieldOfView(NewFOV);

		if (FMath::IsNearlyEqual(NewFOV, 110.0f, 0.01f))
		{
			bRestFOV = false;
		}
	}
	
}

void ACMP302Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	BatteryCount = 5;
	GunAmmo = 30;

	
	OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
	Health = 100.0f;

	
}

//////////////////////////////////////////////////////////////////////////
// Input
void ACMP302Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACMP302Character::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACMP302Character::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACMP302Character::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACMP302Character::Look);

		EnhancedInputComponent->BindAction(PickUp, ETriggerEvent::Started, this, &ACMP302Character::InteractionChange);

		EnhancedInputComponent->BindAction(MiddleButton, ETriggerEvent::Started, this, &ACMP302Character::FlashLightIntensityButton);

		EnhancedInputComponent->BindAction(SlideButton, ETriggerEvent::Started, this, &ACMP302Character::Slide);

		EnhancedInputComponent->BindAction(ShootButton, ETriggerEvent::Started, this, &ACMP302Character::Shoot);
		
		EnhancedInputComponent->BindAction(GrapplingHook, ETriggerEvent::Started, this, &ACMP302Character::GrapplingHookButton);

		EnhancedInputComponent->BindAction(BombButton, ETriggerEvent::Ongoing, this, &ACMP302Character::UpdateTrajectory);
		EnhancedInputComponent->BindAction(BombButton, ETriggerEvent::Completed, this, &ACMP302Character::SpawnBomb);

		EnhancedInputComponent->BindAction(SwitchButton, ETriggerEvent::Completed, this, &ACMP302Character::SwitchToBird);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	
}

void ACMP302Character::Move(const FInputActionValue& Value)
{

	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void ACMP302Character::Slide(const FInputActionValue& Value)
{
	bool SlideButtonInteraction = Value.Get<bool>();

    if (SlideButtonInteraction && !bIsSliding)
	{
		if (UCharacterMovementComponent* characterMovement = GetCharacterMovement())
		{
			bIsSliding = true;
			
			bAdjustingFOV = true;
			FVector SlidingCameraLocation = FollowCamera->GetComponentLocation();
			SlidingCameraLocation.Z -= 8.0f; // Move down by 8 units
			FollowCamera->SetWorldLocation(SlidingCameraLocation);
		
			characterMovement->MaxWalkSpeed = OriginalSpeed * 1.6f;
			
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&ACMP302Character::ReseSlide,5.0f,false);
		}
    	
	}
	
}


void ACMP302Character::ReseSlide()
{
	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	if (characterMovement)
	{
		bIsSliding = false;
		bRestFOV = true;
		FVector SlidingCameraLocation = FollowCamera->GetComponentLocation();
		SlidingCameraLocation.Z += 8.0f; // Move down by 4 units
		FollowCamera->SetWorldLocation(SlidingCameraLocation);
		
		GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;

	}
}


void ACMP302Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACMP302Character::InteractionChange(const FInputActionValue& Value)
{
	
	// input is a Vector2D
	   PickUpDecision = Value.Get<bool>();

	if (PickUpDecision)
	{
		GeneralInteractions();
	}
}

void ACMP302Character::SpawnBomb()
{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		// Spawn parameters
		FActorSpawnParameters SpawnParams;
		// Spawn the bomb actor
		AActor* SpawnedBomb = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

}


void ACMP302Character::SwitchToBird()
{
	
	TSubclassOf<ABird> BirdClass = ABird::StaticClass();
	TArray<AActor*> FoundBirdActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),BirdClass,FoundBirdActors);

	for (AActor* actors : FoundBirdActors)
	{
		ABird* BirdRef = Cast<ABird>(actors);
		
		if (BirdRef)
		{
		
			
			if (GetController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Switched to Bird."));

				GetController()->Possess(BirdRef);
				break; 
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Controller is null. Cannot possess bird."));
			}
		}
	}

}
void ACMP302Character::GeneralInteractions()
{
	TArray<AActor*> Overlappingactors;
	GetOverlappingActors(Overlappingactors);

	for (AActor* Actor : Overlappingactors)
	{

		if(FoundFlashLightRef == false)
		{
			CurrentlyOverlappedActor = Actor;
		}

		if(FoundGuntRef == false)
		{
			OtherCurrentlyOverlappedActor = Actor;
		}
		
		if (Actor)
		{
			InteractableObject = Cast<IInteractableInterface>(Actor);

			if(InteractableObject)
			{
				InteractableObject->InteractWithMe(this);
			}
		}
	}
}

void ACMP302Character::FlashLightIntensityButton(const FInputActionValue& Value)
{
	bool IntensityButton = Value.Get<bool>();

	FlashLight = Cast<AFlashLight>(CurrentlyOverlappedActor);

			if(IntensityButton && FlashLight && CountDown == 5.00)
			{
				
				if(FlashLight)
				{
					FoundFlashLightRef = true;
					
					if(BatteryCount >=0)
					{
						FlashLight->IncreaseLightIntensity();
						FlashLight->PerformLineTraces();
					}
					
					if (BatteryCount < 0)
					{
						BatteryCount = 0;
					}
				}
			}
}


void ACMP302Character::DecreaseTime(float DeltaTime)
{
	CountDown -= DeltaTime;

	if (CountDown < 0)
	{
		ShouldTimerTick = false;
		CountDown = 5.0f;
	}
}

void ACMP302Character::Shoot(const FInputActionValue& Value)
{
	bool IntensityButton = Value.Get<bool>();
	
	Gun = Cast<AGun>(OtherCurrentlyOverlappedActor);

	if(IntensityButton && Gun)
	{
				
		if(Gun)
		{
			FoundGuntRef = true;
					
			if(GunAmmo >0)
			{
				Gun->PerformLineTrace();
				GunAmmo--;
			}
					
					
			if (GunAmmo < 0)
			{
				GunAmmo = 0;
			}
		}
	}
}

void ACMP302Character::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	ISPlayerJumping = true;

}

void ACMP302Character::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
	ISPlayerJumping = false;
}

void ACMP302Character::GrapplingHookButton(const FInputActionValue& Value)
{
	bool GrapplingHookButtonInteractoin = Value.Get<bool>();

	if (GrapplingHookButtonInteractoin )
	{
		GetCharacterMovement()->GravityScale = 1;
		PerformLineTrace();
		bisGrappling = true;
	}

}


void ACMP302Character::PerformLineTrace() 
{
	FVector Start = GetFollowCamera()->GetComponentLocation(); // Starting point of the trace
	Start.Z+= 45;
	FVector ForwardVector = GetFollowCamera()->GetForwardVector(); // Forward direction
	FVector End = Start + (ForwardVector * 1400); // End point of the trace

	FHitResult HitResult; // Structure to hold the result of the trace
	FCollisionQueryParams CollisionParams;

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{

		FVector LaunchParams = {0.0f,0.0f,500.0f};
		LaunchCharacter(LaunchParams,true,true);
		FVector LaunchToTargetPoint  = (HitResult.Location - GetActorLocation()) * 2.5;
		LaunchCharacter(LaunchToTargetPoint,true,true);
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


void ACMP302Character::UpdateTrajectory()
{
	FPredictProjectilePathParams PredictionParams;
	
	FVector StartLocation = GetActorLocation();
	StartLocation.Z+= 15.0f;
	FVector InitialVelocity = GetFollowCamera()->GetForwardVector() * 100 * 10.0;
	
	float Radius = 20.0f;
	float MaxSimTime = 9.0f;
    TArray<AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	PredictionParams.StartLocation = StartLocation;
	PredictionParams.LaunchVelocity = InitialVelocity;
	PredictionParams.ProjectileRadius = Radius;
	PredictionParams.MaxSimTime = MaxSimTime;
	PredictionParams.bTraceComplex = true;
	PredictionParams.bTraceWithCollision = true;  // Enable collision checks
	PredictionParams.SimFrequency = 5.0f;
	PredictionParams.ActorsToIgnore = ActorsToIgnore;
		
	 FPredictProjectilePathResult PredictResult;
     UGameplayStatics::PredictProjectilePath(GetWorld(),PredictionParams,PredictResult);
	
		for (int32 i = 0; i < PredictResult.PathData.Num() - 1; ++i)
		{
			FVector StartPoint = PredictResult.PathData[i].Location;
			FVector EndPoint = PredictResult.PathData[i + 1].Location;

			FString ActorName = PredictResult.HitResult.GetActor()->GetName();
			
			if (GetWorld())
			{
				DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 0.02f, 0, 6.0f);
			}
		}
}
