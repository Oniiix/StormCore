// Fill out your copyright notice in the Description page of Project Settings.


#include "VSyncOption.h"

void UVSyncOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                        const TObjectPtr<UWorld>& _world, const
                        TObjectPtr<USettingsSave>
                        & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	currentIndex = GUS->IsVSyncEnabled();
	Apply();
}

void UVSyncOption::Apply()
{
	Super::Apply();
	TArray<bool> vsyncActive;
	values.GenerateKeyArray(vsyncActive);
	GUS->SetVSyncEnabled(vsyncActive[currentIndex]);
}

TArray<FString> UVSyncOption::GetChoices() const
{
	TArray<FString> choices;
	for (const TTuple<bool, FText>& Value : values)
	{
		FString _value = Value.Value.ToString();
		choices.Add(FORMAT(choiceFormat, _value));
	}
	return choices;
}

void UVSyncOption::ResetToDefault()
{
	currentIndex = static_cast<int>(false);
	Super::ResetToDefault();
}
