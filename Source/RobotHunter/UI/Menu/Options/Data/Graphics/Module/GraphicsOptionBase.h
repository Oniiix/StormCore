// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "GraphicsOptionBase.generated.h"

UENUM()
enum class EGraphicsOptionValue : uint8
{
	LOW = 0 UMETA(DisplayName="Low"),
	MEDIUM UMETA(DisplayName="Medium"),
	HIGH UMETA(DisplayName="High/Far"),
	EPIC UMETA(DisplayName="Epic"),
	CUSTOM UMETA(DisplayName="Custom"),
	NONE = 99 UMETA(DisplayName = "Choose option...")
};

/**
 * 
 */
UCLASS(Abstract)
class ROBOTHUNTER_API UGraphicsOptionBase : public UOptionData
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Global Graphics Settings", meta = (ArraySizeEnum="EGraphicsOptionValue"))
	TMap<EGraphicsOptionValue, FText> OptionsName;
	/**
	 * Default option that will be set during the game
	 */
	UPROPERTY(VisibleAnywhere, Category = "Global Graphics Settings")
	EGraphicsOptionValue defaultOption = EGraphicsOptionValue::NONE;

public:
	FORCEINLINE virtual void SetDefaultValue(const int _value) override
	{
		currentIndex = static_cast<int>(defaultOption = static_cast<EGraphicsOptionValue>(_value));
		Super::SetDefaultValue(currentIndex);
	}

	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
