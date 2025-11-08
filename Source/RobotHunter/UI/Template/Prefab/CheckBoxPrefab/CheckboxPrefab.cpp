// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckboxPrefab.h"

#include "RobotHunter/Utils/DebugUtils.h"


bool UCheckboxPrefab::IsChecked() const
{
	return CheckBox->IsChecked();
}

void UCheckboxPrefab::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION_PARAM(CheckBox, OnCheckStateChanged, UCheckboxPrefab, UpdateCheckbox);
}

void UCheckboxPrefab::LoadStyle() const
{
	if (overrideStyle && CheckBox)
	{
		CheckBox->SetWidgetStyle(overrideStyle->GetStyle());
	}
}

void UCheckboxPrefab::SetOptionParameter(const FString& _name, const TArray<FString>& _list)
{
	text = _name;
	SET_TEXT_STR(titleCheckbox->GetText(), text);
	checkboxTexts = _list;

	CheckBox->SetIsChecked(static_cast<bool>(data->GetCurrentValue()));
	SET_TEXT_STR(checkboxActive->GetText(), IsChecked() ? checkboxTexts[0] : checkboxTexts[1]);
}

#if WITH_EDITOR
void UCheckboxPrefab::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property->GetFName();

	if (PropertyName.ToString().ToLower() != "bDefaultChecked" || (PropertyName.ToString().ToLower() !=
		"overrideStyle"))
	{
		return;
	}

	if (PropertyName.ToString().ToLower() == "bDefaultChecked")
	{
		CheckBox->SetIsChecked(bDefaultChecked);
	}

	/*CheckPropertyOverrideStyle<TObjectPtr<UCheckboxPrefabOverrideStyle>>(
		PropertyChangedEvent, "overrideStyle", overrideStyle,
		bOverrideStyle, [this] { overrideStyle = nullptr; },
		[this] { LoadStyle(); });*/
}
#endif

void UCheckboxPrefab::UpdateCheckbox(bool bIsChecked)
{
	onCheckChange.Broadcast(bIsChecked);
	SET_TEXT_STR(checkboxActive->GetText(), checkboxTexts[bIsChecked ? 0 : 1]);

	Callback_OnChangeDetected();
	Callback_OnChangeIndex(bIsChecked);
}

void UCheckboxPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetEnableWidget<UTextPrefab>(titleCheckbox, bShowTitle);
	SetEnableWidget<UTextPrefab>(checkboxActive, bShowStatus);


	SET_TEXT_STR(titleCheckbox->GetText(), text);
	SET_TEXT_STR(checkboxActive->GetText(), IsChecked() ? checkboxTexts[0] : checkboxTexts[1]);
	CheckBox->SetIsChecked(bDefaultChecked);
	/*if (overrideStyle && bOverrideStyle)
	{
		LoadStyle();
	}*/
}
