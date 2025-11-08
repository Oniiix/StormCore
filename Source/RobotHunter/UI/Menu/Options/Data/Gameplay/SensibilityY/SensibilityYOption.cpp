// Fill out your copyright notice in the Description page of Project Settings.


#include "SensibilityYOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void USensibilityYOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                               const TObjectPtr<UWorld>& _world, const
                               TObjectPtr<USettingsSave>
                               & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	defaultIndex = currentIndex = settings->GetSensibilityY();
	Apply();

	CALLBACK_AFTER_TIME(.2f, &USensibilityYOption::SetSensibilityToPlayer, world);
}

void USensibilityYOption::Apply()
{
	Super::Apply();
	settings->SetSensibilityY(currentIndex);
	SetDefaultValue(currentIndex);

	CALLBACK_AFTER_TIME(.2f, &USensibilityYOption::SetSensibilityToPlayer, world);
}

TArray<FString> USensibilityYOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void USensibilityYOption::ResetToDefault()
{
	currentIndex = settings->GetGameplay().default_sensibilityY;
	Super::ResetToDefault();
}


#if WITH_EDITOR
void USensibilityYOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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

void USensibilityYOption::SetSensibilityToPlayer() const
{
	if (!world) return;
	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}

	_player->SetCameraSensibilityY(currentIndex / 100.f);
}
