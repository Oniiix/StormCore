// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/Settings/PrefabSettings.h"
#include "TextPrefabOverrideStyle.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UTextPrefabOverrideStyle final : public UPrefabSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FLinearColor colorAndOpacity = FLinearColor::White;
	UPROPERTY(EditAnywhere)
	FSlateFontInfo font;
	UPROPERTY(EditAnywhere)
	FSlateBrush strikeBrush;
	UPROPERTY(EditAnywhere)
	FVector2D shadowOffset;
	UPROPERTY(EditAnywhere)
	FLinearColor shadowColorAndOpacity = FLinearColor(0, 0, 0, 0);
	UPROPERTY(EditAnywhere)
	ETextTransformPolicy transformPolicy;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETextJustify::Type> justification;

public:
	FORCEINLINE FLinearColor GetColorAndOpacity() const
	{
		return colorAndOpacity;
	}

	FORCEINLINE FSlateFontInfo GetFont() const
	{
		return font;
	}

	FORCEINLINE FSlateBrush GetStrikeBrush() const
	{
		return strikeBrush;
	}

	FORCEINLINE FVector2D GetShadowOffset() const
	{
		return shadowOffset;
	}

	FORCEINLINE FLinearColor GetShadowColorAndOpacity() const
	{
		return shadowColorAndOpacity;
	}

	FORCEINLINE ETextTransformPolicy GetTransformPolicy() const
	{
		return transformPolicy;
	}

	FORCEINLINE TEnumAsByte<ETextJustify::Type> GetJustification() const
	{
		return justification;
	}
};
