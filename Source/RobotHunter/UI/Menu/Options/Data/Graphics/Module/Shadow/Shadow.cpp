// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow.h"


void UShadowOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                         const TObjectPtr<UWorld>& _world, const
                         TObjectPtr<USettingsSave>
                         & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetShadowQuality();
	SetDefaultValue(_value);
}

void UShadowOption::Apply()
{
	Super::Apply();

	GUS->SetShadowQuality(currentIndex);
}
