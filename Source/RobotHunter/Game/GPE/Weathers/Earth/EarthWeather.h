// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weather.h"
#include "EarthWeather.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AEarthWeather : public AWeather
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float max = 0;
		UPROPERTY(EditAnywhere)
		float min = 0;

public:
	AEarthWeather();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
