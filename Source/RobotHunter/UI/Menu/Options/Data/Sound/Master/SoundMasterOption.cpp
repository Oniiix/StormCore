// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMasterOption.h"

#include "Sound/SoundClass.h"


void USoundMasterOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                              const TObjectPtr<UWorld>& _world, const
                              TObjectPtr<USettingsSave>
                              & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetMasterVolume();
	Apply();
}

void USoundMasterOption::Apply()
{
	Super::Apply();
	settings->SetMasterVolume(currentIndex);
	SetDefaultValue(currentIndex);

	SetSoundGlobal();
}

TArray<FString> USoundMasterOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void USoundMasterOption::ResetToDefault()
{
	currentIndex = settings->GetSound().default_master;
	Super::ResetToDefault();
}

void USoundMasterOption::SetSoundGlobal() const
{
	soundClass->Properties.Volume = currentIndex / 100.f;
}

#if WITH_EDITOR
void USoundMasterOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "minValue" && minValue > maxValue)
		minValue = maxValue;
	if (PropertyName.ToString().ToLower() == "maxValue" && maxValue < minValue)
		maxValue = minValue;
}
#endif
