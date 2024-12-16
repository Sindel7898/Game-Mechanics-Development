// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CableComponent.h"
#include "CMP302Character.generated.h"

class ABomb;
class UBoxComponent;
class USphereComponent;
class AGun;
class IInteractableInterface;
class AFlashLight;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACMP302Character : public ACharacter
{
	GENERATED_BODY()

	/*UPROPERTY(EditAnywhere)
	UBoxComponent* WallDetector;*/
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MiddleButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrapplingHook;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BombButton;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchButton;
	
	bool PickUpDecision;

public:
	ACMP302Character();
	virtual void Tick(float DeltaTime) override;

	int BatteryCount;
	int GunAmmo;

	float CountDown = 5.0f;
	bool ShouldTimerTick = false;
     bool ISPlayerJumping = false;

	 FORCEINLINE UInputMappingContext* GetMappingContext(){return DefaultMappingContext;}
protected:

	virtual void BeginPlay() override;


	/** Called for input actions */
	//////////////////////////////////////////////
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InteractionChange(const FInputActionValue& Value);
	void FlashLightIntensityButton(const FInputActionValue& Value);
	void GrapplingHookButton(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
	void UpdateTrajectory();
	void SpawnBomb();
	void SwitchToBird();
	///////////////////////////////////////////////

	void GeneralInteractions();
	void DecreaseTime(float DeltaTime);
	virtual void Jump() override;
	virtual void StopJumping() override;
	void PerformLineTrace();

    float holddeltatime;
	void ReseSlide();
    bool bIsSliding = false;
	bool bisGrappling = false;
	bool bAdjustingFOV;
	bool bRestFOV;

	 float OriginalSpeed;
	FTimerHandle TimerHandle;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// To add mapping context

	IInteractableInterface* InteractableObject = nullptr;
	UPROPERTY()
	AActor* CurrentlyOverlappedActor;
	UPROPERTY()
	AActor* OtherCurrentlyOverlappedActor;
	UPROPERTY()
	AFlashLight* FlashLight = nullptr;

	
	UPROPERTY(EditAnywhere)
	USceneComponent* RopeStartPoint;
	

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;

	
	UPROPERTY()
	AGun* Gun = nullptr;
	
	bool FoundFlashLightRef = false;
	bool FoundGuntRef = false;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns Battery Count **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetBatteryCount() const { return BatteryCount; }
	
	/** Returns Health Count **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetHealthCount() const { return Health; }

	/** Returns Ammo Count **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetAmmoCount() const { return GunAmmo; }

	float Health; 
};
