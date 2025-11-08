// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonPrefab.h"

#include "Components/ButtonSlot.h"
#include "Components/HorizontalBox.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <RobotHunter/InputSystem/InputSubsystem.h>

#include "CommonInputSubsystem.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"


void UButtonPrefab::SetSelected(const bool _selected)
{
	isSelected = _selected;
	if (_selected)
	{
		if (!buttonOverrideStyle)
		{
			return;
		}
		buttonBase->SetStyle(buttonOverrideStyleSelected->GetStyle());
		buttonBase->SetBackgroundColor(buttonOverrideStyleSelected->GetBackgroundColor());
		buttonBase->SetColorAndOpacity(buttonOverrideStyleSelected->GetColorAndOpacity());
	}
	else
	{
		buttonBase->SetStyle(baseStyle.style);
		buttonBase->SetBackgroundColor(baseStyle.backgroundColor);
		buttonBase->SetColorAndOpacity(baseStyle.colorAndOpacity);
	}
}

void UButtonPrefab::Callback_OnHover()
{
	if (!isSelected)
	{
		Custom_OnHover.Broadcast();
		/*if (UCommonInputSubsystem* CommonInputSystem = UCommonInputSubsystem::Get(GETFPC->GetLocalPlayer()))
		{
			if (CommonInputSystem->GetCurrentInputType() == ECommonInputType::MouseAndKeyboard)
			{
				isFocused = true;
				SetFocus();
				GetGameInstance()->GetSubsystem<UInputSubsystem>()->SetCurrentFocused(this);
			}
		}*/
		if (!hud)		
			hud = Cast<AMainHUD>(GETFPC->GetHUD());
		
		if (INSTANCEOF(hud->GetCurrentUI(), UBaseMenu))
		{
			if (UBaseMenu* _menu = Cast<UBaseMenu>(hud->GetCurrentUI()))
			{
				_menu->SetCurrentSelected(this);
			}				
		}		
	}
}
bool UButtonPrefab::CanKeepSelected()
{
	return bCanBeSelected;
}

void UButtonPrefab::SetStyle(const FButtonStyle& _style) const
{
	buttonBase->SetStyle(_style);
}


void UButtonPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsFocusable(true);
	if (!textBase)
	{
		return;
	}
	textBase->GetText()->SetText(TEXTSTR(FORMAT(textFormat, text.ToString())));

	if (!SB) return;

	UButtonSlot* _slot = Cast<UButtonSlot>(SB->Slot);
	if (!_slot) return;


	_slot->SetHorizontalAlignment(HorizontalAlignment);
	_slot->SetVerticalAlignment(VerticalAlignment);
	_slot->SetPadding(paddingText);

	/*if (bOverrideStyle && textOverrideStyle)
	{
		LoadStyleText();
	}*/
	if (bOverrideStyleButton && buttonOverrideStyle)
	{
		LoadStyleButton();
	}

	baseStyle = FBaseStyleButton(buttonBase->GetStyle(), buttonBase->GetBackgroundColor(),
	                             buttonBase->GetColorAndOpacity());
}

void UButtonPrefab::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	if (isFocused) return;
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	Callback_OnHover();
}

void UButtonPrefab::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	if (GetGameInstance()->GetSubsystem<UInputSubsystem>()->IsGamepad() || !GetGameInstance()->GetSubsystem<
		UInputSubsystem>()->IsGamepad() && isFocused)
		Callback_OnUnHover();
}

FReply UButtonPrefab::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		Callback_OnClick();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UButtonPrefab::DisableButton()
{
	buttonBase->SetIsEnabled(false);
}

void UButtonPrefab::EnableButton()
{
	buttonBase->SetIsEnabled(true);
}

void UButtonPrefab::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION_PARAM(buttonBase, OnClicked, UButtonPrefab, Callback_OnClick);
	BIND_ACTION_PARAM(buttonBase, OnPressed, UButtonPrefab, Callback_OnPressed);
	BIND_ACTION_PARAM(buttonBase, OnReleased, UButtonPrefab, Callback_OnReleased);
	BIND_ACTION_PARAM(buttonBase, OnHovered, UButtonPrefab, Callback_OnHover);
	BIND_ACTION_PARAM(buttonBase, OnUnhovered, UButtonPrefab, Callback_OnUnHover);

	Custom_OnDisabled.AddUniqueDynamic(this, &UButtonPrefab::DisableButton);
	Custom_OnEnabled.AddUniqueDynamic(this, &UButtonPrefab::EnableButton);
	Custom_OnSelected.AddUniqueDynamic(this, &UButtonPrefab::SelectedButton);
	Custom_OnUnSelected.AddUniqueDynamic(this, &UButtonPrefab::UnSelectedButton);
	Custom_OnUnSelected.AddUniqueDynamic(this, &UButtonPrefab::Callback_OnUnHover);
}

void UButtonPrefab::UnSelectedButton()
{
	SetSelected(false);
}

void UButtonPrefab::SelectedButton()
{
	// 1. Retrouver la racine du menu (horizontalMenu)
	UPanelWidget* ParentPanel = Cast<UPanelWidget>(GetParent());
	while (ParentPanel && ParentPanel->GetParent())
	{
		UPanelWidget* PotentialParent = Cast<UPanelWidget>(ParentPanel->GetParent());
		if (!PotentialParent)
			break;

		ParentPanel = PotentialParent;

		if (ParentPanel->GetName() == "horizontalMenu")
			break;
	}

	if (ParentPanel)
	{
		// 2. Fonction récursive pour désélectionner tous les boutons enfants
		auto UnselectAllSelectedButtons = [](UWidget* Widget, auto&& UnselectRef) -> void
		{
			if (UButtonPrefab* Button = Cast<UButtonPrefab>(Widget))
			{
				if (Button->bCanBeSelected && Button->isSelected)
				{
					Button->Callback_OnUnSelected();
				}
			}

			if (UPanelWidget* Panel = Cast<UPanelWidget>(Widget))
			{
				for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
				{
					UWidget* Child = Panel->GetChildAt(i);
					UnselectRef(Child, UnselectRef); // Appel récursif
				}
			}
		};

		// 3. Parcours de tous les enfants de "horizontalMenu"
		for (int32 i = 0; i < ParentPanel->GetChildrenCount(); ++i)
		{
			UWidget* Child = ParentPanel->GetChildAt(i);
			UnselectAllSelectedButtons(Child, UnselectAllSelectedButtons);
		}
	}

	// 4. Sélection du bouton courant
	SetSelected(true);
}

#if WITH_EDITOR

void UButtonPrefab::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	/*CheckPropertyOverrideStyle<TObjectPtr<UDataTable>>(PropertyChangedEvent, "textOverrideStyle",
	                                                                 textOverrideStyle,
	                                                                 bOverrideStyle,
	                                                                 [&]
	                                                                 {
		                                                                 textOverrideStyle = nullptr;
		                                                                 if (!textBase)
		                                                                 {
			                                                                 return;
		                                                                 }
		                                                                 textBase->SetStyle(textOverrideStyle, false);
	                                                                 },
	                                                                 [&] { LoadStyleText(); });*/

	CheckPropertyOverrideStyle<TObjectPtr<UButtonPrefabOverrideStyle>>(PropertyChangedEvent, "buttonOverrideStyle",
	                                                                   buttonOverrideStyle,
	                                                                   bOverrideStyleButton,
	                                                                   [&] { buttonOverrideStyle = nullptr; },
	                                                                   [&]
	                                                                   {
		                                                                   LoadStyleButton();
	                                                                   });
}
#endif


void UButtonPrefab::LoadStyleButton() const
{
	if (!buttonBase || buttonOverrideStyle)
	{
		return;
	}
	buttonBase->SetColorAndOpacity(buttonOverrideStyle->GetColorAndOpacity());
	buttonBase->SetStyle(buttonOverrideStyle->GetStyle());
	buttonBase->SetBackgroundColor(buttonOverrideStyle->GetBackgroundColor());
}

void UButtonPrefab::LoadStyleText() const
{
	if (!textBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error during load style of text [TextBase is null]"));
	}
	/*if (!textOverrideStyle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error during load style of text [Text override  is null]"));
		return;
	}*/
	//textBase->SetStyle(textOverrideStyle, true);
}
