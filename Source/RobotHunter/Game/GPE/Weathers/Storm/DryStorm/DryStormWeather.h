// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StormWeather.h"
#include "DryStormWeather.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ADryStormWeather : public AStormWeather
{
	GENERATED_BODY()
	

public:
	ADryStormWeather();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
