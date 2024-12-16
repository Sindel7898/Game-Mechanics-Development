// Fill out your copyright notice in the Description page of Project Settings.


#include "LightDetection.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ULightDetection::ULightDetection()
{

}


// Called when the game starts or when spawned
void ULightDetection::BeginPlay()
{
	Super::BeginPlay();
	
}


float ULightDetection::CalculateBrightness()
{
	if (detectorTextureTop == nullptr || detectorTextureBottom == nullptr) {
		return 0.0f;
	}
	
	// Reset our values for the next brightness test
	CurrentPixelBrightness = 0;
	BrightnessOutput = 0;

	// Process our top and bottom RenderTextures
	ProcessRenderTexture(detectorTextureTop);
	ProcessRenderTexture(detectorTextureBottom);

	return BrightnessOutput;
}

void ULightDetection::ProcessRenderTexture(UTextureRenderTarget2D* DetectorTexture)
{
	// Read the pixels from our RenderTexture and store the data into our color array
	// Note: ReadPixels is allegedly a very slow operation So this is only done once every 5 frames 
	fRenderTarget = DetectorTexture->GameThread_GetRenderTargetResource();
	fRenderTarget->ReadPixels(PixelStorage);

	//iterate through every pixel  retrieved and find the brightest pixel
	for (int PixelNum = 0; PixelNum < PixelStorage.Num(); PixelNum++) {
		PixelChannelR = PixelStorage[PixelNum].R;
		PixelChannelG = PixelStorage[PixelNum].G;
		PixelChannelB = PixelStorage[PixelNum].B;
		
		CurrentPixelBrightness = ((0.299 * PixelChannelR) + (0.587 * PixelChannelG) + (0.114 * PixelChannelB));

		if (CurrentPixelBrightness >= BrightnessOutput) {
			BrightnessOutput = CurrentPixelBrightness;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Calculations of light percentage
void ULightDetection::CalculatePercentageOfLight()
{
	BrightnessPercentage = Brightness / 140 * 100;
	
	if(BrightnessPercentage > 100 ){
		BrightnessPercentage = 100;
	}
	
}

