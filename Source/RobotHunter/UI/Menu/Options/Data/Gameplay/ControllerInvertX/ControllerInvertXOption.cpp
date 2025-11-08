// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerInvertXOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UControllerInvertXOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                                    const TObjectPtr<UWorld>& _world, const
                                    TObjectPtr<USettingsSave>
                                    & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	currentIndex = static_cast<int>(settings->GetControllerInvertX());
}

void UControllerInvertXOption::Apply()
{
	Super::Apply();
	settings->SetControllerInvertX(static_cast<bool>(currentIndex));
	SetInvertXToPlayer();
}

TArray<FString> UControllerInvertXOption::GetChoices() const
{
	TArray<FString> result;

	for (const TTuple<bool, FText>& Value : values)
	{
		result.Add(FORMAT(choiceFormat, Value.Value.ToString()));
	}

	return result;
}

void UControllerInvertXOption::ResetToDefault()
{
	currentIndex = static_cast<int>(settings->GetGameplay().default_controllerInvertX);
	Super::ResetToDefault();
}

#if WITH_EDITOR
void UControllerInvertXOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove || PropertyChangedEvent.ChangeType ==
		EPropertyChangeType::ArrayClear)
	{
		values = {
			{
				{true, TEXTSTR("Enabled")},
				{false, TEXTSTR("Disabled")}
			}
		};
	}
}
#endif


void UControllerInvertXOption::SetInvertXToPlayer() const
{
	if (!world) return;
	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}

	_player->SetCameraInvertX(static_cast<bool>(currentIndex), true);
}
