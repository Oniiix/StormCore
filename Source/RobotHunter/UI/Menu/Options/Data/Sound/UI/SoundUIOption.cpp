// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundUIOption.h"

#include "Sound/SoundClass.h"


void USoundUIOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                           const TObjectPtr<UWorld>& _world, const
                           TObjectPtr<USettingsSave>
                           & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetSFXVolume();
	Apply();
	CALLBACK_AFTER_TIME(.2f, &USoundUIOption::SetSoundUI, world);
}

void USoundUIOption::Apply()
{
	Super::Apply();
	settings->SetSFXVolume(currentIndex);
	SetDefaultValue(currentIndex);

	SetSoundUI();
}

TArray<FString> USoundUIOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void USoundUIOption::ResetToDefault()
{
	currentIndex = settings->GetSound().default_SFX;
	Super::ResetToDefault();
}

void USoundUIOption::SetSoundUI() const
{
	soundClass->Properties.Volume = currentIndex / 100.f;
}


#if WITH_EDITOR
void USoundUIOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "minValue" && minValue > maxValue)
		minValue = maxValue;
	if (PropertyName.ToString().ToLower() == "maxValue" && maxValue < minValue)
		maxValue = minValue;
}
#endif
