// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/Settings/PrefabSettings.h"
#include "ButtonPrefabOverrideStyle.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UButtonPrefabOverrideStyle final : public UPrefabSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FButtonStyle style = FButtonStyle();
	UPROPERTY(EditAnywhere)
	FLinearColor colorAndOpacity = FLinearColor::White;
	UPROPERTY(EditAnywhere)
	FLinearColor backgroundColor = FLinearColor(0, 0, 0, 0);

public:
	FORCEINLINE FButtonStyle GetStyle() const
	{
		return style;
	}

	FORCEINLINE FLinearColor GetColorAndOpacity() const
	{
		return colorAndOpacity;
	}

	FORCEINLINE FLinearColor GetBackgroundColor() const
	{
		return backgroundColor;
	}
};
