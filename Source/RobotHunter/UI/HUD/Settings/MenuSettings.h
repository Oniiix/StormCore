#pragma once

#include "CoreMinimal.h"
#include "../../BaseSettings.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/ENUM_UI.h"
#include "MenuSettings.generated.h"

#define SETTING_MENU_USE_LOG false

class UUI;
class UBaseFrame;
class AMainHUD;

UCLASS()
class ROBOTHUNTER_API UMenuSettings final : public UBaseSettings
{
	GENERATED_BODY()

	/** The frame of creation (DO NOT TOUCH IF ALREADY SET) */
	UPROPERTY(EditAnywhere, Category = "Custom|Frame")
	TSubclassOf<UBaseFrame> frameReference;

	UPROPERTY()
	UBaseFrame* currentFrame;

	/** All UI of menu. Each state need to have a refenrece of UI */
	UPROPERTY(EditAnywhere, Category = "Custom|UI")
	TMap<EStateUI, TSubclassOf<UUI>> allUIsReference;


	UPROPERTY()
	TMap<EStateUI, TObjectPtr<UUI>> allUIs;

	UPROPERTY(VisibleAnywhere, Category = "Custom|UI")
	TArray<EStateUI> PlayerATH = {EStateUI::PlayerLobbyUI, EStateUI::PlayerGameUI, EStateUI::DraisineUI};

	/** The state of start UI of first frame */
	UPROPERTY(EditAnywhere, Category = "Custom|UI", meta = (GetRestrictedEnumValues="GetStateOfUI"))
	EStateUI startUI;

public:
	/**
	 * Return a boolean with check if state is an ATH of game.
	 * @param _state const EStateUI
	 * @return bool
	 */
	FORCEINLINE bool IsATH(const EStateUI _state) const { return PlayerATH.Contains(_state); }

	/**
	 * Return the state of start UI
	 * @return EstateUI
	 */
	FORCEINLINE EStateUI GetStartUI() const { return startUI; }

	FORCEINLINE TObjectPtr<UUI> GetUI(EStateUI _state) const
	{
		if (!allUIs.Contains(_state))
		{
			LOG_ERROR(true, "allUI doesn't contains : " + GET_ENUM_STRING(_state));
			return nullptr;
		}
		return allUIs[_state];
	}

	/**
	 * Setup HUD in settings to be re-set after in menus
	 * @param _hud AMainHUD*
	 */
	virtual void Setup(const TObjectPtr<AMainHUD>& _hud) override;

	/**
	 * Get the index of state in list of UI
	 * @param _state EStateUI
	 * @return int
	 */
	int GetIndexFromState(EStateUI _state) const;
	/**
	 * Return the current frame
	 * @return UBaseFrame*
	 */
	UBaseFrame* GetCurrentFrame() const;

private:
	/**
	 * Load a the frame of UI
	 * @return bool
	 */
	bool LoadCurrentFrame();
	/**
	 * Load all UI of settings
	 */
	void LoadAllUIs();
	/**
	 * Setup the switcher of UI
	 */
	void SetupSwitcher();

	/**
	 * Get keys set in editor for reference UI 
	 * @return A list of reference enum
	 */
	TArray<EStateUI> GetStateOfUI() const
	{
		TArray<EStateUI> keys;
		allUIsReference.GetKeys(keys);
		return keys;
	}
};
