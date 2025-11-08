// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewDistance.h"


void UViewDistanceOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                               const TObjectPtr<UWorld>& _world, const
                               TObjectPtr<USettingsSave>
                               & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetViewDistanceQuality();
	SetDefaultValue(_value);
}

void UViewDistanceOption::Apply()
{
	Super::Apply();
	GUS->SetViewDistanceQuality(currentIndex);
}
