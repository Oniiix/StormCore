// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalIllumination.h"


void UGlobalIlluminationOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                                     const TObjectPtr<UWorld>& _world, const
                                     TObjectPtr<USettingsSave>
                                     & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetGlobalIlluminationQuality();
	SetDefaultValue(_value);
}

void UGlobalIlluminationOption::Apply()
{
	Super::Apply();
	GUS->SetGlobalIlluminationQuality(currentIndex);
}
