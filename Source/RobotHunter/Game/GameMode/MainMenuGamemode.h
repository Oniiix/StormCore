// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "RobotHunter/UI/Menu/Options/Data/Accessibilty/Lang/LanguageOption.h"
#include "MainMenuGamemode.generated.h"


UCLASS()
class ROBOTHUNTER_API AMainMenuGamemode : public ACustomGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<USettingsSave> Settings;
	UPROPERTY()
	int CurrentIndex;
	UPROPERTY()
	int OldIndex;
	UPROPERTY()
	TMap<EGameLanguage, FLanguage> Language;
	UPROPERTY()
	TObjectPtr<UWorld> World;
	UPROPERTY()
	const ULanguageOption* langObj;

public:
	void SetLang();
	void RestoreLang() const;
	void SetPreLangChange(const ULanguageOption* obj, const TObjectPtr<USettingsSave>& _settings, int _CurrentIndex,
	                      const TMap<EGameLanguage,
	                                 FLanguage>& _Language, const TObjectPtr<UWorld>& _world);
};
