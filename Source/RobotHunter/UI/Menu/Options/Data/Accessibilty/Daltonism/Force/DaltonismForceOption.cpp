// Fill out your copyright notice in the Description page of Project Settings.


#include "DaltonismForceOption.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "RobotHunter/Utils/DebugUtils.h"

void UDaltonismForceOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                                 const TObjectPtr<UWorld>& _world, const
                                 TObjectPtr<USettingsSave>
                                 & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);

	currentIndex = settings ? settings->GetDaltonismForce() : 0;
	Apply();
}

void UDaltonismForceOption::Apply()
{
	Super::Apply();
	CHECK_NULL(settings, "Settings is null");
	settings->SetDaltonismForce(currentIndex);

	UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(
		static_cast<EColorVisionDeficiency>(settings->GetDaltonismType()), currentIndex / 10.f, true, true);
}

TArray<FString> UDaltonismForceOption::GetChoices() const
{
	TArray<FString> _result;

	for (const int ForceDaltonism : forceDaltonism)
	{
		_result.Add(FORMAT(choiceFormat, INTSTR(ForceDaltonism)));
	}
	return _result;
}

void UDaltonismForceOption::ResetToDefault()
{
	LOG_DEBUG("ResetToDefault daltonism");
	currentIndex = settings->GetAccessibility().default_daltonismForce;
	Super::ResetToDefault();
}
