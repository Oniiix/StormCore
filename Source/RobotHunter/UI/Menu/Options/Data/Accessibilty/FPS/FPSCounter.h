// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "FPSCounter.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UFPSCounter final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom",
		meta = (EditCondition = "OptionType == EOptionType::Selector", EditConditionHides))
	TArray<FText> values = {
		{
			TEXTSTR("Off"),
			TEXTSTR("On")
		}
	};

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
		const TObjectPtr<UWorld>& _world, const
		TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;
};
