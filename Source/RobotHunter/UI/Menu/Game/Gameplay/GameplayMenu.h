#pragma once

#include "CoreMinimal.h"
#include "../../BaseMenu.h"
#include "Components/WidgetSwitcher.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "GameplayMenu.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UGameplayMenu final : public UBaseMenu
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> btnInventory;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> btnMap;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> btnCodex;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UWidgetSwitcher> menuSwitcher;

public:
	virtual void RegisterEvents() override;
	virtual void Refresh() override;

	/**
	 * SHow inventory menu
	 */
	UFUNCTION()
	void ShowInventory();
	/**
	 * Show map menu
	 */
	UFUNCTION()
	void ShowMap();
	/**
	 * Show codex menu
	 */
	UFUNCTION()
	void ShowCodex();

private:
	/**
	 * Refresh the switcher with index of UI
	 * @param index index
	 */
	void RefreshUI(int index) const;
	void SetIndex(int index) const;
};
