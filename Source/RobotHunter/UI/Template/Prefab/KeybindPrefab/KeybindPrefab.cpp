#include "KeybindPrefab.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeybindPrefab::SetOptionData(const TObjectPtr<UOptionData>& OptionData)
{
	Super::SetOptionData(OptionData);
	dataKeybind = Cast<UKeybindElement>(OptionData);
	if (!dataKeybind)
	{
		LOG_ERROR(true, "Error during cast dataKeybind");
	}
}

void UKeybindPrefab::SetOptionParameter(const FString& _name, const TArray<FString>& _list)
{
	FInputChord InputChord = CreateInputChordFromFText(_list[0]);
	keySelector->SetSelectedKey(InputChord);

	// Mettre à jour l'affichage selon le type de touche
	UpdateKeyDisplay(InputChord.Key);
}

void UKeybindPrefab::SetCurrentIndexWithoutEvent(const int& _index)
{
	Super::SetCurrentIndexWithoutEvent(_index);
	LOG_DEBUG("Test index reset");
}

void UKeybindPrefab::SetKeyName(const FString& _keyName) const
{
	SET_TEXT_STR(nameKey->GetText(), FORMAT(textFormat, _keyName));
}

void UKeybindPrefab::WaitingKey()
{
	// Afficher le KeySelector pendant la sélection, cacher l'image et son bouton
	if (GamepadTouch && keySelector)
	{
		GamepadTouch->SetVisibility(ESlateVisibility::Hidden);
		if (GamepadTouchButton)
		{
			GamepadTouchButton->SetVisibility(ESlateVisibility::Hidden);
		}
		keySelector->SetVisibility(ESlateVisibility::Visible);
	}
}

void UKeybindPrefab::SetSelectedKey(FInputChord InputChord)
{
	FMapPlayerKeyArgs Args = {};
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetWorld()->GetFirstLocalPlayerFromController()->
		GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
	FKey _key = dataKeybind->GetKey();
	Args.Slot = EPlayerMappableKeySlot::First;
	Args.MappingName = dataKeybind->GetPlayerKeyMapping().GetMappingName();
	Args.NewKey = InputChord.Key;
	dataKeybind->SetNewKey(Args.NewKey);
	FGameplayTagContainer FailureReason;
	UserSettings->UnMapPlayerKey(Args, FailureReason);
	UserSettings->MapPlayerKey(Args, FailureReason);
	UserSettings->SaveSettings();

	if (UInputSubsystem* _subSystem = GetGameInstance()->GetSubsystem<UInputSubsystem>())
	{
		_subSystem->ReplaceKey(Args.MappingName.ToString(), Args.NewKey);
	}

	// Mettre à jour l'affichage selon le type de touche
	UpdateKeyDisplay(InputChord.Key);
}

void UKeybindPrefab::RegisterEvents()
{
	Super::RegisterEvents();
	keySelector->OnIsSelectingKeyChanged.AddUniqueDynamic(this, &UKeybindPrefab::WaitingKey);
	keySelector->OnKeySelected.AddUniqueDynamic(this, &UKeybindPrefab::SetSelectedKey);

	if (GamepadTouchButton)
	{
		GamepadTouchButton->OnKeySelected.AddUniqueDynamic(this, &UKeybindPrefab::OnGamepadImageClicked);
	}
}

FInputChord UKeybindPrefab::CreateInputChordFromFText(const FString& InText) const
{
	FString KeyString = InText;
	TArray<FString> Parts;
	KeyString.ParseIntoArray(Parts, TEXT("+"), true);
	FKey Key;
	bool bShift = false;
	bool bCtrl = false;
	bool bAlt = false;
	bool bCmd = false;

	for (const FString& Part : Parts)
	{
		if (Part.Equals(TEXT("Shift"), ESearchCase::IgnoreCase))
		{
			bShift = true;
		}
		else if (Part.Equals(TEXT("Ctrl"), ESearchCase::IgnoreCase) || Part.Equals(
			TEXT("Control"), ESearchCase::IgnoreCase))
		{
			bCtrl = true;
		}
		else if (Part.Equals(TEXT("Alt"), ESearchCase::IgnoreCase))
		{
			bAlt = true;
		}
		else if (Part.Equals(TEXT("Cmd"), ESearchCase::IgnoreCase) || Part.Equals(
			TEXT("Command"), ESearchCase::IgnoreCase))
		{
			bCmd = true;
		}
		else
		{
			// Assume this is the key
			Key = FKey(*Part);
		}
	}
	return FInputChord(Key, bShift, bCtrl, bAlt, bCmd);
}

void UKeybindPrefab::OnGamepadImageClicked(FInputChord InputChord)
{
	// Quand on clique sur l'image, on force le KeySelector à entrer en mode sélection
	if (keySelector)
	{
		// Cacher l'image et son bouton, afficher le KeySelector
		GamepadTouch->SetVisibility(ESlateVisibility::Hidden);
		if (GamepadTouchButton)
		{
			GamepadTouchButton->SetVisibility(ESlateVisibility::Hidden);
		}
		keySelector->SetVisibility(ESlateVisibility::Visible);
		keySelector->SetSelectedKey(InputChord);
	}
}

void UKeybindPrefab::UpdateKeyDisplay(const FKey& Key)
{
	if (!GamepadTouch || !keySelector)
	{
		return;
	}

	// Vérifier si la touche existe dans le DataAsset
	bool bHasCustomIcon = false;
	if (GamepadIconDataAsset)
	{
		const TMap<FKey, UMaterialInterface*>& KeyMap = GamepadIconDataAsset->GetkeyMap();
		if (UMaterialInterface* const* MaterialPtr = KeyMap.Find(Key))
		{
			if (UMaterialInterface* Material = *MaterialPtr)
			{
				// Touche trouvée dans le DataAsset -> afficher l'image avec le matériau
				GamepadTouch->SetBrushFromMaterial(Material);
				GamepadTouch->SetVisibility(ESlateVisibility::Visible);
				if (GamepadTouchButton)
				{
					GamepadTouchButton->SetVisibility(ESlateVisibility::Visible);
				}
				keySelector->SetVisibility(ESlateVisibility::Hidden);
				bHasCustomIcon = true;
			}
		}
	}

	if (!bHasCustomIcon)
	{
		// Touche non trouvée dans le DataAsset -> afficher le nom de la touche
		GamepadTouch->SetVisibility(ESlateVisibility::Hidden);
		if (GamepadTouchButton)
		{
			GamepadTouchButton->SetVisibility(ESlateVisibility::Hidden);
		}
		keySelector->SetVisibility(ESlateVisibility::Visible);
	}
}