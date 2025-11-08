// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weather.h"
#include "WindWeather.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AWindWeather : public AWeather
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float maxIntensity = 10;
	UPROPERTY(EditAnywhere)
		float minIntensity = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
		float currentIntensity;

	UPROPERTY(EditAnywhere)
		FVector direction;



public:
	AWindWeather();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FVector SelectDirection();
};
