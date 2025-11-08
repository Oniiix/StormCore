#pragma once

#include "CoreMinimal.h"
#include "../BaseMenu.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "PauseMenu.generated.h"

#define PAUSE_MENU_USE_LOG false

class UOptionFrame;
class UOptionMenu;
class UCustomGameInstance;
class UHorizontalBox;

UENUM()
enum class EStatusPause : uint8
{
	Game = 0, Friends, Options, Map
};

UCLASS(HideDropdown)
class ROBOTHUNTER_API UPauseMenu final : public UBaseMenu
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRefreshPause, EStatusPause, _statusPause);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShow);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuit);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowMap);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideMap);

	/**
	 * Event called when menu refresh first time
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow onShow;
	/*
	 * Event called when the "Map" button is clicked
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHideMap onHideMap;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShowMap onShowMap;

	/**
	 * Event called when menu refresh first time
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnRefreshPause onRefreshPause;

	/**
	 * Event called when menu is quit
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow onQuit;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UWidgetSwitcher> menuSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> Pause_BtnGame;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> btnFriends;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> Pause_btnMap;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> horizontalMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> backgroundMenu;

	UPROPERTY()
	TObjectPtr<UCustomGameInstance> GameInstance;

public:
	FORCEINLINE TObjectPtr<UButtonPrefab> GetBtnMap() const { return Pause_btnMap; }
	FORCEINLINE FOnShowMap& GetOnShowMap() { return onShowMap; }
	FORCEINLINE FOnHideMap& GetOnHideMap() { return onHideMap; }

	UFUNCTION()
	void ShowOption();
	void ShowGame();

	virtual void Refresh() override;

	virtual void DisableAllWidgetInteractable() override;

protected:
	virtual void Init() override;
	virtual void RegisterEvents() override;

private:
	UFUNCTION()
	void QuitMenu();
	UFUNCTION()
	void ShowMap();
	UFUNCTION()
	void ShowFriends();
	UFUNCTION()
	void ClearSelected();

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (AllowPrivateAccess))
	UOptionFrame* GetOption() const;

	void RefreshUI(int index) const;
};
