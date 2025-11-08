// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflections.h"


void UReflectionOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                             const TObjectPtr<UWorld>& _world, const
                             TObjectPtr<USettingsSave>
                             & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetReflectionQuality();
	SetDefaultValue(_value);
}

void UReflectionOption::Apply()
{
	Super::Apply();
	GUS->SetReflectionQuality(currentIndex);
}
