// 


#include "KeyInput.h"

#include "Components/InputKeySelector.h"

void UKeyInput::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (textPrefab)
		SET_TEXT_STR(textPrefab->GetText(), FORMAT(formatText, Text.ToString()));
	KeySelector->SetSelectedKey(KeySelected);
	KeySelector->SetIsEnabled(false);
	sizeBoxWidget->SetWidthOverride(size);
	gamepadImage->SetBrushFromTexture(textureGamepad);
}

void UKeyInput::SwitchToController() const
{
	ControllerSwitcher->SetActiveWidgetIndex(1);
}

void UKeyInput::SwitchToKeyboard() const
{
	ControllerSwitcher->SetActiveWidgetIndex(0);
}

void UKeyInput::InitWidget(const FKey& Key, const FText& _Text, const FString& String,
                           const TObjectPtr<UTexture2D>& Object)
{
	KeySelected = Key;
	Text = _Text;
	size = 300;
	formatText = String;
	textureGamepad = Object;
}
