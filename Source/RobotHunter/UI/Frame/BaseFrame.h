#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "BaseFrame.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UBaseFrame final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UWidgetSwitcher> switcher;

public:
	/**
	 * Show the frame in HUD
	 */
	void Show();
	/**
 * hide the frame in HUD
 */
	void Hide();
	/**
	 * Check if widget has parented
	 * @return bool
	 */
	FORCEINLINE bool HasParent() const
	{
		return GetParent() != nullptr;
	}

	/**
	 * Get the switcher of UI (most optimized)
	 * @return UWidgetSwitcher*
	 */
	TObjectPtr<UWidgetSwitcher> GetSwitcher() const;
};
