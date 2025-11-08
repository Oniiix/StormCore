// Fill out your copyright notice in the Description page of Project Settings.


#include "FOVOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UFOVOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                      const TObjectPtr<UWorld>& _world, const
                      TObjectPtr<USettingsSave>
                      & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);


	defaultIndex = currentIndex = settings->GetFOV();
	Apply();
}

void UFOVOption::Apply()
{
	Super::Apply();
	settings->SetFOV(currentIndex);

	if (!world)
	{
		return;
	}
	if (!world->GetFirstPlayerController())
	{
		return;
	}

	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}
	UCustomCameraComponent* _camera = Cast<UCustomCameraComponent>(
		_player->GetComponentByClass(UCustomCameraComponent::StaticClass()));
	if (!_camera)
	{
		return;
	}
	_camera->SetFOV(currentIndex);
}

TArray<FString> UFOVOption::GetChoices() const
{
	return {FORMAT(choiceFormat, INTSTR(minValue)), FORMAT(choiceFormat, INTSTR(maxValue))};
}

void UFOVOption::ResetToDefault()
{
	currentIndex = settings->GetGameplay().default_fov;
	Super::ResetToDefault();
}


#if WITH_EDITOR
void UFOVOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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
