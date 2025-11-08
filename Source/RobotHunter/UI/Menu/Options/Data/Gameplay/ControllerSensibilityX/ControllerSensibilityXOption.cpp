// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerSensibilityXOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UControllerSensibilityXOption::Init(const TObjectPtr<UOptionFrame>& _parent,
                                         const TObjectPtr<UGameUserSettings>& _gus,
                                         const TObjectPtr<UWorld>& _world, const
                                         TObjectPtr<USettingsSave>
                                         & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetControllerSensibilityX();
	Apply();
	CALLBACK_AFTER_TIME(.2f, &UControllerSensibilityXOption::SetSensibilityToPlayer, world);
}

void UControllerSensibilityXOption::Apply()
{
	Super::Apply();
	settings->SetControllerSensibilityX(currentIndex);
	SetDefaultValue(currentIndex);

	CALLBACK_AFTER_TIME(.2f, &UControllerSensibilityXOption::SetSensibilityToPlayer, world);
}

TArray<FString> UControllerSensibilityXOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void UControllerSensibilityXOption::ResetToDefault()
{
	currentIndex = settings->GetGameplay().default_controllerSensibilityX;
	Super::ResetToDefault();
}


#if WITH_EDITOR
void UControllerSensibilityXOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "minValue" && minValue > maxValue)
	{
		minValue = maxValue;
	}
	if (PropertyName.ToString().ToLower() == "maxValue" && maxValue < minValue)
	{
		maxValue = minValue;
	}
}
#endif


void UControllerSensibilityXOption::SetSensibilityToPlayer() const
{
	if (!world) return;
	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}

	_player->SetCameraSensibilityX(currentIndex / 100.f, true);
}
