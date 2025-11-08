// Fill out your copyright notice in the Description page of Project Settings.


#include "Foliage.h"


void UFoliageOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                          const TObjectPtr<UWorld>& _world, const
                          TObjectPtr<USettingsSave>
                          & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	int _value = GUS->GetFoliageQuality();
	SetDefaultValue(_value);
}

void UFoliageOption::Apply()
{
	Super::Apply();
	GUS->SetFoliageQuality(currentIndex);
}
