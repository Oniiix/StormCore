// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerSensibilityYOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UControllerSensibilityYOption::Init(const TObjectPtr<UOptionFrame>& _parent,
                                         const TObjectPtr<UGameUserSettings>& _gus,
                                         const TObjectPtr<UWorld>& _world, const
                                         TObjectPtr<USettingsSave>
                                         & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetControllerSensibilityY();
	Apply();

	CALLBACK_AFTER_TIME(.2f, &UControllerSensibilityYOption::SetSensibilityToPlayer, world);
}

void UControllerSensibilityYOption::Apply()
{
	Super::Apply();
	settings->SetControllerSensibilityY(currentIndex);
	SetDefaultValue(currentIndex);

	CALLBACK_AFTER_TIME(.2f, &UControllerSensibilityYOption::SetSensibilityToPlayer, world);
}

TArray<FString> UControllerSensibilityYOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void UControllerSensibilityYOption::ResetToDefault()
{
	currentIndex = settings->GetGameplay().default_controllerSensibilityY;
	Super::ResetToDefault();
}


#if WITH_EDITOR
void UControllerSensibilityYOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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

void UControllerSensibilityYOption::SetSensibilityToPlayer() const
{
	if (!world) return;
	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}

	_player->SetCameraSensibilityY(currentIndex / 100.f, true);
}
