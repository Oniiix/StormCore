// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundSFXOption.h"

#include "Sound/SoundClass.h"


void USoundSFXOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                           const TObjectPtr<UWorld>& _world, const
                           TObjectPtr<USettingsSave>
                           & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetSFXVolume();
	Apply();
	CALLBACK_AFTER_TIME(.2f, &USoundSFXOption::SetSoundSFX, world);
}

void USoundSFXOption::Apply()
{
	Super::Apply();
	settings->SetSFXVolume(currentIndex);
	SetDefaultValue(currentIndex);

	SetSoundSFX();
}

TArray<FString> USoundSFXOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void USoundSFXOption::ResetToDefault()
{
	currentIndex = settings->GetSound().default_SFX;
	Super::ResetToDefault();
}

void USoundSFXOption::SetSoundSFX() const
{
	soundClass->Properties.Volume = currentIndex / 100.f;
}


#if WITH_EDITOR
void USoundSFXOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "minValue" && minValue > maxValue)
		minValue = maxValue;
	if (PropertyName.ToString().ToLower() == "maxValue" && maxValue < minValue)
		maxValue = minValue;
}
#endif
