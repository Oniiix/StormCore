// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSOption.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UFPSOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                      const TObjectPtr<UWorld>& _world, const
                      TObjectPtr<USettingsSave>
                      & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	GUS->SetFrameRateLimit(_settings->GetFPS());

	/*UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "FPS Limit: " + INTSTR(GUS->GetFrameRateLimit()));*/
	
	defaultIndex = currentIndex = GetIndexSelection(_settings->GetFPS());
	Apply();
}

void UFPSOption::Apply()
{
	Super::Apply();
	settings->SetFPS(fpsMaxValue[currentIndex]);
	GUS->SetFrameRateLimit(fpsMaxValue[currentIndex]);
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "Apply FPS: " + INTSTR(fpsMaxValue[currentIndex]));
}

TArray<FString> UFPSOption::GetChoices() const
{
	TArray<FString> choices;
	for (int FPSMaxValue : fpsMaxValue)
	{
		choices.Add(FORMAT(choiceFormat, INTSTR(FPSMaxValue)));
	}
	return choices;
}

void UFPSOption::ResetToDefault()
{
	
	currentIndex = fpsMaxValue.IndexOfByKey(settings->GetGraphics().default_fps);
	Super::ResetToDefault();
}

int UFPSOption::GetIndexSelection(const int _framerate) const
{
	if (!fpsMaxValue.Contains(_framerate))
	{
		return fpsMaxValue.Num() - 1;
	}

	int index = 0;
	for (int FPSMaxValue : fpsMaxValue)
	{
		if (FPSMaxValue == _framerate)
		{
			return index;
		}
		index++;
	}
	return index;
}
