// Fill out your copyright notice in the Description page of Project Settings.


#include "InputFieldPrefab.h"

#include "RobotHunter/Utils/DebugUtils.h"

void UInputFieldPrefab::TextChange(const FText& Text)
{
	onTextChange.Broadcast(Text.ToString());
}

void UInputFieldPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();
	//inputField->SetHintText(TEXTSTR(FORMAT(textFormat, text.ToString())));
	if (bOverrideStyle && overrideStyle)
	{
		LoadStyle();
	}
}

void UInputFieldPrefab::RegisterEvents()
{
	Super::RegisterEvents();

	BIND_ACTION_PARAM(inputField, OnTextChanged, UInputFieldPrefab, TextChange);
}

void UInputFieldPrefab::LoadStyle() const
{
	if (inputField)
	{
		inputField->WidgetStyle = overrideStyle->GetStyle();
	}
}

#if WITH_EDITOR
void UInputFieldPrefab::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckPropertyOverrideStyle<TObjectPtr<UInputFieldPrefabOverrideStyle>>(
		PropertyChangedEvent, "overrideStyle", overrideStyle,
		bOverrideStyle, [this] { overrideStyle = nullptr; },
		[this] { LoadStyle(); });
}
#endif WITH_EDITOR
