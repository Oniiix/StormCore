// 


#include "MainMenuGamemode.h"

#include "RobotHunter/Game/CustomGameInstance.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"
#include <Kismet/KismetInternationalizationLibrary.h>

#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"

class UCustomGameInstance;

void AMainMenuGamemode::SetLang()
{
	Settings->SetLanguageSelection(CurrentIndex);
	
	const int _size = Language.Num();
	for (int i = 0; i < _size; i++)
	{
		if (CurrentIndex == i)
		{
			EGameLanguage _enum = static_cast<EGameLanguage>(i);
			if (Language.Contains(_enum))
			{
				const FLanguage _option = Language[_enum];
				//World->GetGameInstance<UCustomGameInstance>()->CallbackOnLanguageChange(_option.shortcut);
				UKismetInternationalizationLibrary::SetCurrentLanguage(_option.shortcut);
				GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->SaveSettings();
			}
			break;
		}
	}
}


void AMainMenuGamemode::RestoreLang() const
{
	const TObjectPtr<UOptionablePrefab> _option = langObj->GetCurrentPrefab();
	if (!_option) return;
	_option->SetCurrentIndexWithoutEvent(OldIndex);
	_option->GetData()->SetHasChanged(false);
	_option->GetData()->SetDefaultValue(OldIndex);
}

void AMainMenuGamemode::SetPreLangChange(const ULanguageOption* obj, const TObjectPtr<USettingsSave>& _settings,
                                         int _CurrentIndex,
                                         const TMap<EGameLanguage, FLanguage>& _Language,
                                         const TObjectPtr<UWorld>& _world)
{
	langObj = obj;
	Settings = _settings;
	CurrentIndex = _CurrentIndex;
	OldIndex = _settings->GetLanguageSelection();
	Language = _Language;
	World = _world;
}
