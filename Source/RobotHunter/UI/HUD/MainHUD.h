#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RobotHunter/UI/Template/Popup/PopupConfirmation.h"
#include "RobotHunter/UI/Template/Popup/Asset/PopupData.h"
#include "Settings/MenuSettings.h"
#include "Settings/FocusSystem.h"
#include "MainHUD.generated.h"

#define MAIN_HUD_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	// --- Events / Delegates ---
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateUIChange);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelReload);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelReload onLevelReload;


	// --- Prefabs & Data (Popups) ---
	UPROPERTY(EditAnywhere, Category = "Custom|Prefab")
	TSubclassOf<UPopupConfirmation> popupPrefab;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom|Popup", meta = (AllowPrivateAccess))
	TMap<FString, UPopupConfirmation*> Popups;

	UPROPERTY(EditAnywhere, Category = "Custom|Popup", Blueprintable, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TMap<FString, TObjectPtr<UPopupData>> PopupDatas;

	// --- Settings ---
	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMenuSettings> menuSettings;


	/** This is focus system. Just create data asset with focus system and put here */
	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UFocusSystem> focusSystem;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Custom|UI")
	EStateUI currentUI = EStateUI::None;

public:
	// --- Getters ---
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFocusSystem* GetFocusSystem() const { return focusSystem; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UBaseFrame* GetCurrentFrame() const { return menuSettings->GetCurrentFrame(); }

	EStateUI GetState() const { return currentUI; }

	UFUNCTION(BlueprintCallable)
	bool IsInLoading() const { return currentUI == EStateUI::Loading; }

	// --- Switchers / Setters ---
	virtual void BeginPlay() override;

	/**
	 *  Change the state of ui with switcher
	 * @param _state EStateUI
	 */
	UFUNCTION(BlueprintCallable)
	UUI* SetState(EStateUI _state);

	// --- UI Interaction ---
	void Regenerate();

	UFUNCTION(BlueprintCallable, Category = "Custom|HUD")
	virtual void HideUI();

	UFUNCTION(BlueprintCallable, Category = "Custom|HUD")
	virtual void ShowUI();

	// --- Popup Handling ---
	UPopupConfirmation* CreatePopup(const FString& _key);
	TMap<FString, TObjectPtr<UPopupData>> GetPopupDatas() const { return PopupDatas; }

	UFUNCTION(BlueprintCallable, Category = "Custom|Popup", meta = (AllowPrivateAccess))
	FORCEINLINE UPopupConfirmation* GetPopup(const FString& _id)
	{
		if (Popups.Contains(_id))
		{
			return Popups[_id];
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (AllowPrivateAccess))
	FString GetKeyFromPopupData(const UPopupData* _data) const
	{
		if (!_data) return "";
		for (const TTuple<FString, TObjectPtr<UPopupData>>& PopupData : PopupDatas)
		{
			if (PopupData.Value == _data) return PopupData.Key;
		}
		return "";
	}
	// --- UI Component Access ---
	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (AllowPrivateAccess))
	UUI* GetCurrentUI() const
	{
		CHECK_NULL_WITH_RETURN(menuSettings, "Menu settings is null", nullptr);
		return menuSettings->GetUI(currentUI);
	}

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (AllowPrivateAccess))
	UUI* GetUI(EStateUI _state) const
	{
		CHECK_NULL_WITH_RETURN(menuSettings, "Menu settings is null", nullptr);
		return menuSettings->GetUI(_state);
	}

protected:
	// --- Virtual overrides ---
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// --- Popup Data Access ---
	UFUNCTION(BlueprintCallable, Category = "Custom|Popup", meta = (AllowPrivateAccess))
	FORCEINLINE UPopupData* GetPopupDataFromId(const FString& _id)
	{
		if (PopupDatas.Contains(_id))
		{
			return PopupDatas[_id];
		}
		return nullptr;
	}

	

	// --- Popup Management ---
	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (AllowPrivateAccess))
	void ClosePopup(const FString& _namePopup);

	// --- Utility / Internal ---
	// TODO: revoir avec Thib pour seamless
	UFUNCTION()
	void OnLoadFinish(EStateUI StateUI);
	/**
	 * Restart the game to apply and reload language
	 */
	UFUNCTION(BlueprintCallable)
	void RestartGame();
	/**
	 * Set the old language selected
	 */
	UFUNCTION(BlueprintCallable)
	void RestoreLanguage();

public:
	// --- Templates ---
	template <typename T>
	TObjectPtr<T> GetCurrentUI() const;

	template <typename T>
	TObjectPtr<T> GetUI(const EStateUI& _state) const;
};

// --- Templates Implementation ---
template <typename T>
TObjectPtr<T> AMainHUD::GetCurrentUI() const
{
	return Cast<T>(menuSettings->GetUI(currentUI));
}

template <typename T>
TObjectPtr<T> AMainHUD::GetUI(const EStateUI& _state) const
{
	return Cast<T>(menuSettings->GetUI(_state));
}
