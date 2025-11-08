#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"
#include "Components/Image.h"
#include "Engine/DataAsset.h"
#include "Components/Button.h"
#include "RobotHunter/UI/Menu/Options/Data/Keybind/KeybindElement.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "RobotHunter/UI/ATH/Game/Cooldown/KeyImageDataAsset.h"

#include "KeybindPrefab.generated.h"

/**
 *
 */
UCLASS()
class ROBOTHUNTER_API UKeybindPrefab final : public UOptionablePrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> nameKey;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UInputKeySelector> keySelector;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> GamepadTouch;

	// Bouton invisible pour capturer les clics sur l'image
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UInputKeySelector> GamepadTouchButton;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString textFormat = "<title>{0}</>";

	// Référence vers votre DataAsset contenant les icônes gamepad
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom" , meta = (AllowPrivateAccess))
	TObjectPtr<UKeyImageDataAsset> GamepadIconDataAsset;

	UPROPERTY()
	UKeybindElement* dataKeybind;

public:
	virtual void SetOptionData(const TObjectPtr<UOptionData>& OptionData) override;
	virtual void SetOptionParameter(const FString& _name, const TArray<FString>& _list) override;
	virtual void SetCurrentIndexWithoutEvent(const int& _index) override;

	void SetKeyName(const FString& _keyName) const;

private:
	UFUNCTION()
	void WaitingKey();
	UFUNCTION()
	void SetSelectedKey(FInputChord InputChord);
	UFUNCTION()
	void OnGamepadImageClicked(FInputChord InputChord);

	virtual void RegisterEvents() override;

	FInputChord CreateInputChordFromFText(const FString& InText) const;

	// Nouvelle fonction pour gérer l'affichage selon la présence dans le DataAsset
	void UpdateKeyDisplay(const FKey& Key);
};