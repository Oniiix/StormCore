// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureOption.h"


void UTextureOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                          const TObjectPtr<UWorld>& _world, const
                          TObjectPtr<USettingsSave>
                          & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetTextureQuality();
	SetDefaultValue(_value);
}

void UTextureOption::Apply()
{
	Super::Apply();

	GUS->SetTextureQuality(currentIndex);
}
