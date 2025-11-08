// Fill out your copyright notice in the Description page of Project Settings.


#include "PresetGraphics.h"

#include "RobotHunter/UI/Menu/Options/OptionFrame.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"


void UPresetGraphicsOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                                 const TObjectPtr<UWorld>& _world, const
                                 TObjectPtr<USettingsSave>
                                 & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);

	FTimerHandle handle;
	_world->GetTimerManager().SetTimer(handle, [&]{ RefreshValue(); },	0.1f, false, 1.0f);
	
	Apply();
}

void UPresetGraphicsOption::Apply()
{
	Super::Apply();
}

void UPresetGraphicsOption::RefreshValue()
{
	const TArray<int> _values = GetAllValuesGraphicsOptions();
	const float _average = GetAverage(_values);
	currentIndex = static_cast<int>(GetValueFromAverage(_average));
	if (currentPrefab)
	{
		currentPrefab->SetCurrentIndexWithoutEvent(currentIndex);
	}
}

TArray<int> UPresetGraphicsOption::GetAllValuesGraphicsOptions() const
{
	TArray<int> _values;
	const TObjectPtr<UOptionSettings> _settings = parent->GetSettings();
	FOption& _option = _settings->GetOptionByCategory(ECategoryOption::GRAPHICS);
	for (const FOptionCategory& Option : _option.options)
	{
		for (const TObjectPtr<UOptionData>& OptionData : Option.options)
		{
			if (OptionData == this)
			{
				continue;
			}
			if (!INSTANCEOF(OptionData, UGraphicsOptionBase))
			{
				continue;
			}
			if (INSTANCEOF(OptionData, UPresetGraphicsOption))
			{
				continue;
			}
			_values.Add(OptionData->GetCurrentValue());
		}
	}
	return _values;
}

EGraphicsOptionValue UPresetGraphicsOption::GetValueFromAverage(const float& _average) const
{
	const float _decimal = _average - static_cast<int>(_average);

	EGraphicsOptionValue _option;
	if (_decimal != 0.0f)
	{
		_option = EGraphicsOptionValue::CUSTOM;
	}
	else
	{
		_option = static_cast<EGraphicsOptionValue>(static_cast<int>(_average));
	}
	return _option;
}

float UPresetGraphicsOption::GetAverage(const TArray<int>& _values) const
{
	float sum = 0;
	for (const int& Value : _values)
	{
		sum += Value;
	}

	return sum / _values.Num();
}
