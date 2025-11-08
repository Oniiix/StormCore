#pragma once

#include "CoreMinimal.h"
#include "Asset/PopupData.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPopup/ButtonPopupPrefab.h"
#include "PopupConfirmation.generated.h"

class USpacer;
class UButtonPrefab;
class UHorizontalBox;
class UTextPrefab;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UPopupConfirmation final : public UPrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpen);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuit);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnOpen onOpen;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnOpen onQuit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textPopup;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> Horizontal;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> Close;

	UPROPERTY(EditAnywhere, Category = "Custom|Reference")
	TSubclassOf<USpacer> spacerReference;

	/** Prefab button to create with option */
	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UButtonPopupPrefab> buttonSelectionPrefab;

	UPROPERTY()
	TMap<UButtonPrefab*, FVoidDelegate> ButtonToKeyMap;

	UPROPERTY(BlueprintReadOnly, meta= (AllowPrivateAccess))
	UPopupData* data;
	UPROPERTY()
	TMap<FString, FVoidDelegate> Callbacks;

	UPROPERTY()
	UButtonPopupPrefab* ButtonClicked;

public:
	FORCEINLINE void SetClickedButton(UButtonPopupPrefab* _button) { ButtonClicked = _button; }
	UFUNCTION(BlueprintCallable, Category = "Custom|Popup")
	void Hide();
	UFUNCTION(BlueprintCallable, Category = "Custom|Popup")
	void ClearFunctions();
	UFUNCTION(BlueprintCallable, Category = "Custom|Popup")
	void Show(const bool _withCloseButton = true);


	void HideWithoutEvent();
	void Setup(const TObjectPtr<UPopupData>& _popupData);

private:
	UFUNCTION()
	void OnButtonClicked();

	virtual void RegisterEvents() override;
};
