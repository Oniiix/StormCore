// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weather.h"
#include "BlueSkyWeather.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ABlueSkyWeather : public AWeather
{
	GENERATED_BODY()

	

public:
	ABlueSkyWeather();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
