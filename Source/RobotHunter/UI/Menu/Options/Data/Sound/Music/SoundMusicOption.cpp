// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMusicOption.h"

#include "Sound/SoundClass.h"


void USoundMusicOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                             const TObjectPtr<UWorld>& _world, const
                             TObjectPtr<USettingsSave>
                             & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetMusicVolume();
	Apply();
}

void USoundMusicOption::Apply()
{
	Super::Apply();
	settings->SetMusicVolume(currentIndex);
	SetDefaultValue(currentIndex);

	SetSoundMusic();
}

TArray<FString> USoundMusicOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void USoundMusicOption::ResetToDefault()
{
	currentIndex = settings->GetSound().default_music;
	Super::ResetToDefault();
}

void USoundMusicOption::SetSoundMusic()
{
	//LOG_DEBUG("Print sound volume: " + FLOATSTR(currentIndex / 100.f));
	soundClass->Properties.Volume = currentIndex / 100.f;
}


#if WITH_EDITOR
void USoundMusicOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "minValue" && minValue > maxValue)
		minValue = maxValue;
	if (PropertyName.ToString().ToLower() == "maxValue" && maxValue < minValue)
		maxValue = minValue;
}
#endif
