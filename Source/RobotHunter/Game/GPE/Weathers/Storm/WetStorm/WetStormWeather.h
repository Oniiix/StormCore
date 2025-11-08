// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StormWeather.h"
#include "WetStormWeather.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AWetStormWeather : public AStormWeather
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
