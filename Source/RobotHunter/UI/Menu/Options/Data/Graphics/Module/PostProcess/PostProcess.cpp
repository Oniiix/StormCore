// Fill out your copyright notice in the Description page of Project Settings.


#include "PostProcess.h"


void UPostProcessOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                              const TObjectPtr<UWorld>& _world, const
                              TObjectPtr<USettingsSave>
                              & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetPostProcessingQuality();
	SetDefaultValue(_value);
}

void UPostProcessOption::Apply()
{
	Super::Apply();

	GUS->SetPostProcessingQuality(currentIndex);
}
