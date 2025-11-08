// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weather.h"
#include "RobotHunter/Game/Managers/LightningGenerator/LightningGenerator.h"
#include "StormWeather.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AStormWeather : public AWeather
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Weather|Storm", meta = (Units="Seconds"))
	float lightningFrequency = 3;

	UPROPERTY(EditAnywhere, Category = "Weather|Storm")
	TSubclassOf<ULightningGenerator> lightningGeneratorRef = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Weather|Storm", meta = (Instanced))
	TObjectPtr<ULightningGenerator> lightningGenerator = nullptr;


protected:
	virtual void BeginPlay() override;
};
