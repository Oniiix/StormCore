// Fill out your copyright notice in the Description page of Project Settings.


#include "AimToggleOption.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UAimToggleOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                            const TObjectPtr<UWorld>& _world, const
                            TObjectPtr<USettingsSave>
                            & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	currentIndex = static_cast<int>(settings->GetAimHolding());
	Apply();
}

void UAimToggleOption::Apply()
{
	Super::Apply();
	settings->SetAimHolding(static_cast<bool>(currentIndex));
	SetAimTogglePlayer();
}

TArray<FString> UAimToggleOption::GetChoices() const
{
	TArray<FString> result;

	for (const TTuple<bool, FText>& Value : values)
	{
		result.Add(FORMAT(choiceFormat, Value.Value.ToString()));
	}

	return result;
}

void UAimToggleOption::ResetToDefault()
{
	currentIndex = static_cast<int>(settings->GetGameplay().default_aimHolding);
	Super::ResetToDefault();
}

#if WITH_EDITOR
void UAimToggleOption::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove || PropertyChangedEvent.ChangeType ==
		EPropertyChangeType::ArrayClear)
	{
		values = {
			{
				{true, TEXTSTR("Holding")},
				{false, TEXTSTR("Toggle")}
			}
		};
	}
}
#endif


void UAimToggleOption::SetAimTogglePlayer() const
{
	if (!world) return;
	const TObjectPtr<AMainCharacter> _player = Cast<AMainCharacter>(world->GetFirstPlayerController()->GetPawn());
	if (!_player)
	{
		return;
	}

	_player->SetHoldScope(static_cast<bool>(currentIndex));
}
