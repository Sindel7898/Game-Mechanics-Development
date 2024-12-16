// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnrealClient.h"
#include "LightDetection.generated.h"

UCLASS()
class  ULightDetection : public USceneComponent
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	ULightDetection();
	

	void ProcessRenderTexture(UTextureRenderTarget2D *texture);

	TArray<FColor> PixelStorage;
	float PixelChannelR = 0;
	float PixelChannelG = 0;
	float PixelChannelB = 0;
	float BrightnessOutput = 0;
	float CurrentPixelBrightness = 0;

	FRenderTarget *fRenderTarget;

	FString CurrentLevel;

	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D *detectorTextureTop;
	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D *detectorTextureBottom;
	

	UPROPERTY(EditAnywhere)
	bool BLighting;
	float Brightness;
	float BrightnessPercentage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBrightness() const {return BrightnessPercentage;};
	float CalculateBrightness();
	void CalculatePercentageOfLight();

};
