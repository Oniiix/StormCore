// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DropData.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UDropData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float price = 0;
	UPROPERTY(EditAnywhere)
	float weight = 10;
	UPROPERTY(EditAnywhere)
	float maxWeight = 125;


public:
	FORCEINLINE float GetPrice() { return price; }
	FORCEINLINE float GetWeight() { return weight; }
	FORCEINLINE float GetMaxWeight() { return maxWeight; }

};
