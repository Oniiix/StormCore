// Fill out your copyright notice in the Description page of Project Settings.


#include "Shading.h"


void UShadingOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                          const TObjectPtr<UWorld>& _world, const
                          TObjectPtr<USettingsSave>
                          & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetShadingQuality();
	SetDefaultValue(_value);
}

void UShadingOption::Apply()
{
	Super::Apply();

	GUS->SetShadingQuality(currentIndex);
}
