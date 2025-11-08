// Fill out your copyright notice in the Description page of Project Settings.


#include "FullScreenOption.h"

void UFullScreenOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                             const TObjectPtr<UWorld>& _world, const
                             TObjectPtr<USettingsSave>
                             & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	const EWindowMode::Type _window = _gus->GetFullscreenMode();
	currentIndex = GetIndexFromTypeWindow(_window);
	Apply();
}

void UFullScreenOption::Apply()
{
	Super::Apply();
	GUS->SetFullscreenMode(GetWindowFromIndex(currentIndex));
}

TArray<FString> UFullScreenOption::GetChoices() const
{
	TArray<FString> choices;
	for (const TTuple<TEnumAsByte<EWindowMode::Type>, FText>& WindowMode : TypeWindowMode)
	{
		FString _value = WindowMode.Value.ToString();
		choices.Add(FORMAT(choiceFormat, _value));
	}
	return choices;
}

void UFullScreenOption::ResetToDefault()
{
	TArray<TEnumAsByte<EWindowMode::Type>> Keys;
	TypeWindowMode.GetKeys(Keys);

	currentIndex = Keys.IndexOfByKey(defaultIndex);
	Super::ResetToDefault();
}

int UFullScreenOption::GetIndexFromTypeWindow(EWindowMode::Type Window) const
{
	short index = 0;
	for (const TTuple<TEnumAsByte<EWindowMode::Type>, FText>& WindowMode : TypeWindowMode)
	{
		if (WindowMode.Key == Window)
		{
			return index;
		}
		index++;
	}
	return 0;
}

#if WITH_EDITOR

void UFullScreenOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "TypeWindowMode")
	{
		if (TypeWindowMode.Num() == 0)
		{
			TypeWindowMode =
			{
				{EWindowMode::Type::Fullscreen, TEXTSTR("FullScreen")},
				{EWindowMode::Type::WindowedFullscreen, TEXTSTR("Window FullScreen")},
				{EWindowMode::Type::Windowed, TEXTSTR("Windowed")}
			};
		}
	}
}
#endif
