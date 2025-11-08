// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "ControllerSensibilityYOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UControllerSensibilityYOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Sensibility,
		meta = (EditCondition = "OptionType == EOptionType::Slider", EditConditionHides))
	float minValue = 0.1f;
	UPROPERTY(EditAnywhere, Category = Sensibility,
		meta = (EditCondition = "OptionType == EOptionType::Slider", EditConditionHides))
	float maxValue = 200.f;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void SetSensibilityToPlayer() const;
};
