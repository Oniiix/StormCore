// Fill out your copyright notice in the Description page of Project Settings.


#include "AntiAliasingOption.h"

#include "GameFramework/GameUserSettings.h"


void UAntiAliasingOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                               const TObjectPtr<UWorld>& _world, const
                               TObjectPtr<USettingsSave>
                               & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	currentIndex = GUS->GetAntiAliasingQuality();
	Apply();
}

void UAntiAliasingOption::Apply()
{
	Super::Apply();
	GUS->SetAntiAliasingQuality(currentIndex);
}
