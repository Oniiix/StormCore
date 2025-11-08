// Fill out your copyright notice in the Description page of Project Settings.


#include "InvertYOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UInvertYOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                          const TObjectPtr<UWorld>& _world, const
                          TObjectPtr<USettingsSave>
                          & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	currentIndex = static_cast<int>(settings->GetInvertY());
}

void UInvertYOption::Apply()
{
	Super::Apply();
	settings->SetInvertY(static_cast<bool>(currentIndex));
	SetInvertYToPlayer();
}

TArray<FString> UInvertYOption::GetChoices() const
{
	TArray<FString> result;

	for (const TTuple<bool, FText>& Value : values)
	{
		result.Add(FORMAT(choiceFormat, Value.Value.ToString()));
	}

	return result;
}

void UInvertYOption::ResetToDefault()
{
	currentIndex = settings->GetGameplay().default_invertY;
	Super::ResetToDefault();
}

#if WITH_EDITOR
void UInvertYOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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


void UInvertYOption::SetInvertYToPlayer() const
{
	if (!world) return;
	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}

	_player->SetCameraInvertY(static_cast<bool>(currentIndex));
}
