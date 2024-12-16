#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Bird.generated.h"

class UInputMappingContext;
class UInputAction;
class UMaterialInstance;
class ACMP302Character;
class ASimpleAIController;
class UBehaviorTree;

UCLASS()
class CMP302_API ABird : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABird();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* BirdDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BirdMoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BirdLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AccendButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DescendButton;

	FTimerHandle RandomMovementTimer;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY()
	ASimpleAIController* AIController;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Ascend(const FInputActionValue& Value);
	void Descend(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SwitchToPlayer();
	void CheckifBirdLookingAtEnemy();
	void MoveToRandomLocation();
	FVector GetRandomLocationWithinRadius(float Radius);
	virtual  void PossessedBy(AController* NewController)override;
	UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}

	UPROPERTY()
	ACMP302Character* PlayerRef;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* EnemyDefultMat;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* EnemyDefultMat2;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* EmmisiveMaterial;
};
