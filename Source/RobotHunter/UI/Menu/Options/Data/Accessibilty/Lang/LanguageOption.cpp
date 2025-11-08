// Fill out your copyright notice in the Description page of Project Settings.


#include "LanguageOption.h"

#include "Kismet/KismetInternationalizationLibrary.h"
#include "RobotHunter/Game/GameMode/MainMenuGamemode.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/MainMenu/MainMenu/MainMenu.h"

void ULanguageOption::Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
                           const TObjectPtr<UWorld>& _world, const
                           TObjectPtr<USettingsSave>
                           & _settings)
{
	Super::Init(_parent, _gus, _world, _settings);

	currentIndex = settings ? settings->GetLanguageSelection() : 0;
	
	AMainMenuGamemode* _gm = Cast<AMainMenuGamemode>(world->GetAuthGameMode());
	CHECK_NULL(_gm, "Gamemode main menu is null");

	_gm->SetPreLangChange(this, settings, currentIndex, LanguageOptions, world);
	_gm->SetLang();
	//Apply();
}

void ULanguageOption::Apply()
{
	if (!hasChanged) return;
	Super::Apply();
	if (!settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings is null"));
		return;
	}
	AMainMenuGamemode* _gm = Cast<AMainMenuGamemode>(world->GetAuthGameMode());
	CHECK_NULL(_gm, "Gamemode main menu is null");

	_gm->SetPreLangChange(this, settings, currentIndex, LanguageOptions, world);

	if (world->GetFirstPlayerController())
	{
		//CHECK_NULL(popupIntro, "Popup Intro asset is null");
		AMainHUD* _hud = Cast<AMainHUD>(world->GetFirstPlayerController()->GetHUD());
		if (UPopupConfirmation* _popup = _hud->GetPopup(_hud->GetKeyFromPopupData(restartData)))
			_popup->Show(false);
	}
}

TArray<FString> ULanguageOption::GetChoices() const
{
	TArray<FLanguage> _result;
	LanguageOptions.GenerateValueArray(_result);

	TArray<FString> _res;
	for (const FLanguage& Result : _result)
		_res.Add(FORMAT(choiceFormat, Result.langName.ToString()));
	return _res;
}

void ULanguageOption::ResetToDefault()
{
	//LOG_DEBUG("Reset default lang");
	//Super::ResetToDefault();
	//currentIndex = settings->GetAccessibility().default_langSelection;
	//Apply();
}
