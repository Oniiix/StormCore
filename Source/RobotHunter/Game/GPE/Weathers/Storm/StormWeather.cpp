// Fill out your copyright notice in the Description page of Project Settings.


#include "StormWeather.h"
#include "RobotHunter/Utils/DebugUtils.h"

void AStormWeather::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		return;

	/// Create lightningGenerator
	if (!lightningGeneratorRef)
	{
		LOG_ERROR(true, "lightningGeneratorRef is null !!");
	}
	else
	{
		lightningGenerator = NewObject<ULightningGenerator>(this, lightningGeneratorRef);
		if (lightningGenerator)
		{
			lightningGenerator->SetLightningStrikeFrequency(lightningFrequency);
			lightningGenerator->StartGenerateLightning();
		}
		else
			LOG_ERROR(true, "Failed to initialize lightningGenerator!!");
	}
}
