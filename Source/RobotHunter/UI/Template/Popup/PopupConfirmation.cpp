// 


#include "PopupConfirmation.h"

#include "Asset/PopupData.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPopup/ButtonPopupPrefab.h"


class USpacer;

void UPopupConfirmation::Setup(const TObjectPtr<UPopupData>& _popupData)
{
	Horizontal->ClearChildren();

	data = _popupData;

	CALLBACK_LAMBDA_AFTER_TIME(1.0f, SET_TEXT_STR(textPopup->GetText(), data->GetText()););


	TArray<FString> _keys;
	CHECK_NULL(_popupData, "Popup data is null");
	_popupData->GetEvents().GetKeys(_keys);
	for (const FString& CallbackKey : _keys)
	{
		Callbacks.Add(CallbackKey, _popupData->GetEvents()[CallbackKey]);
	}

	const int size = Callbacks.Num();
	int count = 0;
	for (const TTuple<FString, FVoidDelegate>& Callback : Callbacks)
	{
		count++;
		UButtonPopupPrefab* _prefab = CreateWidget<UButtonPopupPrefab>(this, buttonSelectionPrefab);
		CHECK_NULL_WITH_RETURN(_prefab, "prefab button popup for " + Callback.Key + " is null", continue);

		Horizontal->AddChildToHorizontalBox(_prefab);
		UHorizontalBoxSlot* _hbSlot = Cast<UHorizontalBoxSlot>(_prefab->Slot);
		_hbSlot->SetSize(ESlateSizeRule::Fill);

		if (!_prefab) return;
		_prefab->Setup(this, Callback.Key);

		ButtonToKeyMap.Add(_prefab, Callback.Value);
		BIND_ACTION(_prefab, OnClick, UPopupConfirmation, OnButtonClicked);

		if (count == size)
		{
			continue;
		}
		CHECK_NULL_WITH_RETURN(spacerReference, "Spacer is null reference", continue);
		TObjectPtr<USpacer> _spacer = NewObject<USpacer>(this, spacerReference);
		CHECK_NULL_WITH_RETURN(_spacer, "Spacer is null", continue);
		_spacer->SetSize(FVector2D(_popupData->GetSpaceBetweenElement(), 0));
		Horizontal->AddChild(_spacer);
	}
}

void UPopupConfirmation::OnButtonClicked()
{
	CHECK_NULL(ButtonClicked, "ButtonClicked is null");
	if (FVoidDelegate* Delegate = ButtonToKeyMap.Find(ButtonClicked))
	{
		if (!Delegate->ExecuteIfBound())
		{
			LOG_ERROR(true, "Delegate is null");
		}
	}
}

void UPopupConfirmation::Hide()
{
	onQuit.Broadcast();
}

void UPopupConfirmation::ClearFunctions()
{
	ButtonToKeyMap.Empty();
	Callbacks.Empty();
}

void UPopupConfirmation::HideWithoutEvent()
{
	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UPopupConfirmation::Show(const bool _withCloseButton)
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
	Close->SetVisibility(_withCloseButton ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Close->SetIsEnabled(_withCloseButton);
	onOpen.Broadcast();
}

void UPopupConfirmation::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(Close, OnClick, UPopupConfirmation, Hide);
}
