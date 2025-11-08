// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "CustomExponentialHeightFogComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Rendering, collapsecategories, hidecategories = (Object, Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UCustomExponentialHeightFogComponent : public UExponentialHeightFogComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable) void SetVolumetricFogStartDistance(const float _newValue);
	UFUNCTION(BlueprintCallable) void SetVolumetricFogNearFadeInDistance(const float _newValue);
	
};
