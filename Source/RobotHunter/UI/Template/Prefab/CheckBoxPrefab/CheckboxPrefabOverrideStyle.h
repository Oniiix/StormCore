// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/Settings/PrefabSettings.h"
#include "CheckboxPrefabOverrideStyle.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UCheckboxPrefabOverrideStyle final : public UPrefabSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FCheckBoxStyle style;

public:
	FORCEINLINE FCheckBoxStyle GetStyle() const
	{
		return style;
	}
};
