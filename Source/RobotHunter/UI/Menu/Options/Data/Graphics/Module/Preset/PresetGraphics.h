// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/Graphics/Module/GraphicsOptionBase.h"
#include "PresetGraphics.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, HideDropdown)
class ROBOTHUNTER_API UPresetGraphicsOption final : public UGraphicsOptionBase
{
	GENERATED_BODY()

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;

	void RefreshValue();

private:
	float GetAverage(const TArray<int>& _values) const;
	TArray<int> GetAllValuesGraphicsOptions() const;
	EGraphicsOptionValue GetValueFromAverage(const float& _average) const;
};
