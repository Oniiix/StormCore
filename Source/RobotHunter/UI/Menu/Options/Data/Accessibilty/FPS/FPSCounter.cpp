// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCounter.h"

#include "RobotHunter/Game/GameMode/MainMenuGamemode.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/UI/Menu/MainMenu/MainMenu/MainMenu.h"

void UFPSCounter::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	const TObjectPtr<UWorld>& _world, const
	TObjectPtr<USettingsSave>& _settings)
{
	Super::Init(_parent, _gus, _world, _settings);

	currentIndex = settings ? static_cast<int>(settings->GetShowFPS()) : 0;
	defaultIndex = currentIndex;
	Apply();
}

void UFPSCounter::Apply()
{
	Super::Apply();
	if (!settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings is null"));
		return;
	}
	UCustomGameInstance* _gm = Cast<UCustomGameInstance>(world->GetGameInstance());
	CHECK_NULL(_gm, "Gamemode main menu is null");

	_gm->OnFPSShow().Broadcast(static_cast<bool>(currentIndex));
	settings->SetShowFPS(static_cast<bool>(currentIndex));
}

TArray<FString> UFPSCounter::GetChoices() const
{
	TArray<FString> _res;
	for (const FText& Result : values)
		_res.Add(FORMAT(choiceFormat, Result.ToString()));
	return _res;
}

void UFPSCounter::ResetToDefault()
{
	currentIndex = settings->GetAccessibility().default_fpsShow;
	Super::ResetToDefault();
	
}
