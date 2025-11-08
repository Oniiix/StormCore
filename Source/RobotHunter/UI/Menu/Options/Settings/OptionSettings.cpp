// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionSettings.h"

#include "RobotHunter/UI/Menu/Options/Data/Graphics/Module/Preset/PresetGraphics.h"
#include "RobotHunter/Utils/FileLogger.h"


TArray<FString> UOptionSettings::GetCategoryName() const
{
	TArray<FString> _names = {};
	for (TPair _categoryNameByEnum : categoryNameByEnum)
	{
		_names.Add(FORMAT(textFormat, _categoryNameByEnum.Value.name.ToString()));
	}
	return _names;
}

TArray<ECategoryOption> UOptionSettings::GetCategory() const
{
	TArray<ECategoryOption> _category = {};
	for (TPair _categoryNameByEnum : categoryNameByEnum)
	{
		_category.Add(_categoryNameByEnum.Key);
	}
	return _category;
}

void UOptionSettings::LoadOptions(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                                  const TObjectPtr<UWorld>& _world, const TObjectPtr<USettingsSave>& _settings)
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Load options settings");
	TObjectPtr<UOptionData> _preset = nullptr;
	for (TPair<ECategoryOption, FOption>& _categoryNameByEnum : categoryNameByEnum)
	{
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
		                      "Load category: " + UFileLogger::Format(GET_ENUM_STRING(_categoryNameByEnum.Key)));
		for (FOptionCategory& _category : _categoryNameByEnum.Value.options)
		{
			UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Load all options in category");
			_category.options.Empty();
			for (const TSubclassOf<UOptionData>& _optionsReference : _category.optionsReference)
			{
				if (!_optionsReference)
				{
					UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(),
					                      "Options reference is null");
				}
				CHECK_NULL_WITH_RETURN(_optionsReference,
				                       "_optionsReference is null in " + _categoryNameByEnum.Value.name.ToString(),
				                       continue);

				TObjectPtr<UOptionData> _data = NewObject<UOptionData>(this, _optionsReference);
				UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
				                      "Create object for option data");
				CHECK_NULL_WITH_RETURN(_data, "_data is null", continue);

				if (INSTANCEOF(_data, UPresetGraphicsOption))
				{
					_preset = _data;
				}
				else
				{
					_category.options.Add(_data);
				}
				_data->Init(_parent, _gus, _world, _settings);
			}
			if (_preset)
			{
				_category.options.Insert(_preset, 0);
				_preset = nullptr;
			}
		}
	}
}

void UOptionSettings::ResetOptions(const ECategoryOption _category)
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "Reset category: " + GET_ENUM_STRING(_category));

	const TArray<FOptionCategory> _options = categoryNameByEnum[_category].options;

	for (const FOptionCategory& Option : _options)
	{
		for (const TObjectPtr<UOptionData>& OptionData : Option.options)
		{
			OptionData->Reset();
		}
	}
}
