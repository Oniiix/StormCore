// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "FpsOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, HideDropdown)
class ROBOTHUNTER_API UFPSOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom",
		meta = (EditCondition = "OptionType == EOptionType::Selector", EditConditionHides))
	TArray<int> fpsMaxValue;
	UPROPERTY(EditAnywhere, Category = "Custom",
		meta = (EditCondition = "OptionType == EOptionType::Slider", EditConditionHides, UIMin = 10, ClampMin = 10))
	int maxFpsValue;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

private:
	int GetIndexSelection(const int _framerate) const;
};
