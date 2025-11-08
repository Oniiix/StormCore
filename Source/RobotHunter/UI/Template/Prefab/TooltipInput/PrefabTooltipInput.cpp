// 


#include "PrefabTooltipInput.h"

#include "CommonInputSubsystem.h"
#include "Components/HorizontalBoxSlot.h"
#include "RobotHunter/UI/Template/Prefab/KeyInput/KeyInput.h"


void UPrefabTooltipInput::NativePreConstruct()
{
	Super::NativePreConstruct();
	NativeConstruct();
}

void UPrefabTooltipInput::NativeConstruct()
{
	Super::NativeConstruct();


	hbTooltipInput->ClearChildren();
	for (const FKeyTooltip& Key : keys)
	{
		UKeyInput* InputChild = CreateWidget<UKeyInput>(this, referenceKeyInput);
		CHECK_NULL(InputChild, "Input child is null");

		InputChild->InitWidget(Key.key, Key.name, Key.format, Key.imageGamepadKey);
		hbTooltipInput->AddChildToHorizontalBox(InputChild);
		UHorizontalBoxSlot* _slot = Cast<UHorizontalBoxSlot>(InputChild->Slot);
		CHECK_NULL(_slot, "Slot is null");
		_slot->SetPadding(FMargin(10, 10, 0, 10));
		CachedInputWidgets.Add(InputChild);
	}
}

void UPrefabTooltipInput::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto* LocalPlayer = GETFPC->GetLocalPlayer();
	if (!LocalPlayer) return;

	if (UCommonInputSubsystem* InputSys = UCommonInputSubsystem::Get(LocalPlayer))
	{
		bool CurrentIsGamepad = InputSys->GetCurrentInputType() == ECommonInputType::Gamepad;
		if (isGamepad != CurrentIsGamepad)
		{
			isGamepad = CurrentIsGamepad;
			for (UKeyInput* InputWidget : CachedInputWidgets)
			{
				if (isGamepad)
					InputWidget->SwitchToController();
				else
					InputWidget->SwitchToKeyboard();
			}
		}
	}
}
