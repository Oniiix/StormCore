// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionData.h"

#include "GameFramework/GameUserSettings.h"

void UOptionData::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                       const TObjectPtr<UWorld>& _world, const
                       TObjectPtr<USettingsSave>& _settings)
{
	parent = _parent;
	GUS = _gus;
	world = _world;
	settings = _settings;

	if (!parent)
	{
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(), "parent variable is null");
	}
	if (!GUS)
	{
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(), "GUS variable is null");
	}
	if (!world)
	{
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(), "world variable is null");
	}
	if (!settings)
	{
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(), "settings variable is null");
	}
}

int UOptionData::GetCurrentValue() const
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "Get current value: " + INTSTR(currentIndex));
	return currentIndex;
}

void UOptionData::ResetToDefault()
{
	Apply();
	defaultIndex = currentIndex;
}

void UOptionData::SetPrefab(const TObjectPtr<UOptionablePrefab>& _currentPrefab)
{
	currentPrefab = _currentPrefab;
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "Link to prefab " + UFileLogger::GetClassName(_currentPrefab.GetClass()));
}

void UOptionData::Apply()
{
	defaultIndex = currentIndex;
	hasChanged = false;
}

void UOptionData::ChangeValue(const int& _value)
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "change value from " + INTSTR(currentIndex) + " to " + INTSTR(_value));
	currentIndex = _value;
	hasChanged = true;
}

void UOptionData::ExecuteCommand(const FString& _cmd) const
{
	GetWorld()->Exec(GetWorld(), *_cmd);
}
