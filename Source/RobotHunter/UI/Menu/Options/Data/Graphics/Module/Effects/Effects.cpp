// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects.h"


void UEffectOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                         const TObjectPtr<UWorld>& _world, const
                         TObjectPtr<USettingsSave>
                         & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetVisualEffectQuality();
	SetDefaultValue(_value);
}

void UEffectOption::Apply()
{
	Super::Apply();
	GUS->SetVisualEffectQuality(currentIndex);
}
