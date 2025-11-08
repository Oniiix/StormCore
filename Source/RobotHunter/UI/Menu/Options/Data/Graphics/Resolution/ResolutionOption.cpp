// Fill out your copyright notice in the Description page of Project Settings.


#include "ResolutionOption.h"

#include "RobotHunter/Utils/DebugUtils.h"

void UResolutionOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                             const TObjectPtr<UWorld>& _world, const
                             TObjectPtr<USettingsSave>
                             & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	const FIntPoint _resolution = _gus->GetScreenResolution();
	currentIndex = GetIndexOfResolution(_resolution);
	Apply();
}

void UResolutionOption::Apply()
{
	Super::Apply();
	GUS->SetScreenResolution(Resolutions[currentIndex]);
}

TArray<FString> UResolutionOption::GetChoices() const
{
	const FString format = "{0}";
	TArray<FString> choices;
	for (const FIntPoint& Resolution : Resolutions)
	{
		choices.Add(FORMAT(choiceFormat, static_cast<int>(Resolution.X), static_cast<int>(Resolution.Y)));
	}
	return choices;
}

void UResolutionOption::ResetToDefault()
{
	currentIndex = defaultIndexValue;
	Super::ResetToDefault();
}

int UResolutionOption::GetIndexOfResolution(const FIntPoint& resolution)
{
	int index = 0;
	for (const FIntPoint& _resolution : Resolutions)
	{
		if (_resolution == resolution)
		{
			return index;
		}
		index++;
	}
	return 0;
}

#if WITH_EDITOR

void UResolutionOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "Resolutions")
	{
		if (Resolutions.Num() == 0)
		{
			Resolutions =
			{
				{1920, 1080},
				{1280, 720},
				{854, 480},
				{640, 360},
				{426, 240},
			};
		}
	}
}
#endif
