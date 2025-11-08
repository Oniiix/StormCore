// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/Settings/PrefabSettings.h"
#include "ScrollboxPrefabOverrideStyle.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UScrollboxPrefabOverrideStyle final : public UPrefabSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FScrollBoxStyle style;
	UPROPERTY(EditAnywhere)
	FScrollBarStyle barStyle;

public:
	FORCEINLINE FScrollBoxStyle GetStyleScroll() const
	{
		return style;
	}

	FORCEINLINE FScrollBarStyle GetStyleBar() const
	{
		return barStyle;
	}
};
