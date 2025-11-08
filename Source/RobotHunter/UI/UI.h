#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/FileLogger.h"

#include "UI.generated.h"

class UInputSubsystem;
class UButtonPrefab;
class UPrefab;
class AMainHUD;


/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UUI : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRefresh);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInit);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitWithDelay);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRegisterEvents);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnInit onInit;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnInitWithDelay onInitWithDelay;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnRegisterEvents onRegisterEvents;
		
	
	
protected:
	/**
	 * Event called when a menu showed
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnRefresh onRefresh;
	
	/**
		 * hud global variable instancied in setup method
		 */
	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	TObjectPtr<AMainHUD> hud;

	UPROPERTY()
	TArray<UWidget*> widgetToDisabled;

	UPROPERTY(BlueprintReadOnly, Category = "Custom")
	UUI* ParentClass;
	
	UPROPERTY()
	TObjectPtr<UInputSubsystem> inputSubsystem;


	/**
	 * Method can override to init class
	 * Called after NativeConstruct
	 */
	virtual void Init();
	/**
	 * Method can override to register events class
	 */
	virtual void RegisterEvents();

	virtual void InitWithDelay();


	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UFUNCTION()
	FORCEINLINE void Callback_OnRefresh() { onRefresh.Broadcast(); }
public:
	
	/**
	 * Set if the widget is enabled or not. Set visibile to value and enable to value
	 * @tparam Widget Widget Type
	 * @param _widget widget to enable/disable
	 * @param _enable value
	 */
	template <typename Widget>
	void SetEnableWidget(const TObjectPtr<Widget>& _widget, bool _enable);
	virtual void SetParent(UUI* _parent);
	FORCEINLINE FOnRefresh& OnRefresh() { return onRefresh; }
	FORCEINLINE AMainHUD* GetHud() const { return hud; }
	//FORCEINLINE TObjectPtr<UButtonPrefab> GetMainButton() const { return MainButton; }
	
	/**
	 * Method to refresh UI when switched on this
	 */
	UFUNCTION()
	virtual void Refresh();

	/**
	 * Native construct of widget, to init and register automatic
	 */
	virtual void NativeConstruct() override;

	/**
		 * Disable oll interactable widget
		 */
	virtual void DisableAllWidgetInteractable()
	{
		for (UWidget* Widget : widgetToDisabled)
		{
			Widget->SetIsEnabled(false);
		}
	}

	/**
	 * Setup this class with HUD in param (to be access in children)
	 * @param _hud
	 */
	void Setup(const TObjectPtr<AMainHUD>& _hud);

	virtual TObjectPtr<UWidget> GetFirstWidget();
	virtual TObjectPtr<UWidget> GetLastWidget();

	UFUNCTION()
	virtual void HandleInputChanged(bool isGamepad);
	virtual void ExitUI();
};

template <typename Widget>
void UUI::SetEnableWidget(const TObjectPtr<Widget>& _widget, bool _enable)
{
	if (INSTANCEOF(_widget, UUserWidget))
	{
		_widget->SetVisibility(_enable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_widget->SetIsEnabled(_enable);
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
		                      FString(_enable ? "Enabled" : "Disabled") + " Widget apply");
	}
}
