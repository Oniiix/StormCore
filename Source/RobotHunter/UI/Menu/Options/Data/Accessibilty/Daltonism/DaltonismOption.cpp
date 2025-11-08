// Fill out your copyright notice in the Description page of Project Settings.


#include "DaltonismOption.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "RobotHunter/Utils/DebugUtils.h"

void UDaltonismOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                            const TObjectPtr<UWorld>& _world, const
                            TObjectPtr<USettingsSave>
                            & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);

	currentIndex = settings ? settings->GetDaltonismType() : 0;
	Apply();
}

void UDaltonismOption::Apply()
{
	Super::Apply();
	CHECK_NULL(settings, "Settings is null");
	settings->SetDaltonismType(currentIndex);

	UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(static_cast<EColorVisionDeficiency>(currentIndex),
	                                                      settings->GetDaltonismForce(), true, true);
}

TArray<FString> UDaltonismOption::GetChoices() const
{
	TArray<FText> _result;
	daltonismText.GenerateValueArray(_result);

	TArray<FString> _res;
	for (const FText& Result : _result) _res.Add(FORMAT(choiceFormat, Result.ToString()));
	return _res;
}

void UDaltonismOption::ResetToDefault()
{
	LOG_DEBUG("ResetToDefault daltonism");
	currentIndex = settings->GetAccessibility().default_daltonismType;
	Super::ResetToDefault();
}
