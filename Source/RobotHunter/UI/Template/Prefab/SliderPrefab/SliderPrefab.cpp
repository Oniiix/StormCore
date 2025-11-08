// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderPrefab.h"

#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/Options/OptionMenu.h"
#include "RobotHunter/UI/Menu/Options/Data/Graphics/Module/Preset/PresetGraphics.h"

void USliderPrefab::SetCurrentIndex(const int& _index)
{
	Super::SetCurrentIndex(_index);
	SET_TEXT_STR(InputValue->GetInputField(), INTSTR(sliderWidget->GetValue()));
	Callback_OnChangeDetected();
	Callback_OnChangeIndex(_index);
}

void USliderPrefab::SetCurrentIndexWithoutEvent(const int& _index)
{
	sliderWidget->SetValue(_index);
	if (AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD()))
	{
		if (UOptionMenu* _option =  _hud->GetCurrentUI<UOptionMenu>())
		{
			UOptionFrame* _frame = _option->GetOptionFrame();
			_frame->SetEnableWidget<UButtonPrefab>(_frame->GetApplyButton(), false);
		}
	}
	Super::SetCurrentIndexWithoutEvent(_index);
}

void USliderPrefab::SetOptionParameter(const FString& _name, const TArray<FString>& _list)
{
	text = _name;
	SET_TEXT_STR(title->GetText(), text);
	minValue = STR_TO_INT(ExtractNumberFromRichText(_list[0]));
	maxValue = STR_TO_INT(ExtractNumberFromRichText(_list[1]));
	sliderWidget->SetMinValue(minValue);
	sliderWidget->SetMaxValue(maxValue);
	sliderWidget->SetValue(data->GetCurrentValue());
	SET_TEXT_STR(InputValue->GetInputField(), INTSTR(sliderWidget->GetValue()));
}

void USliderPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!title)
	{
		return;
	}

	SetEnableWidget<UTextPrefab>(title, hasTitle);

	SET_TEXT_STR(title->GetText(), text);
	//title->SetStyle(overrideStyleTitle, bOverrideStyleTitle);
}

void USliderPrefab::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION_PARAM(sliderWidget, OnValueChanged, USliderPrefab, ChangeValue);
	BIND_ACTION(InputValue, OnTextChange, USliderPrefab, ChangeValueByText);
}

void USliderPrefab::ChangeValue(float _value)
{
	const int _val = static_cast<int>(_value);
	SetCurrentIndex(CLAMP(_val, minValue, maxValue));
}

void USliderPrefab::ChangeValueByText(const FString& _value)
{
	const FString _text = _value;
	if (!_text.IsNumeric())
	{
		SET_TEXT_STR(InputValue->GetInputField(), _text.Mid(0, _text.Len() - 1));
		return;
	}
	const int _val = FMath::Clamp(STR_TO_INT(_value), minValue, maxValue);
	SET_TEXT_STR(InputValue->GetInputField(), INTSTR(_val));
	sliderWidget->SetValue(_val);
}

FString USliderPrefab::ExtractNumberFromRichText(const FString& _richText) const
{
	int32 OpenBracketPos;
	int32 CloseBracketPos;
	if (_richText.FindChar('<', OpenBracketPos) && _richText.FindChar('>', CloseBracketPos))
	{
		if (CloseBracketPos > OpenBracketPos)
		{
			return _richText.Mid(CloseBracketPos + 1).Replace(TEXT("</>"), TEXT(""));
		}
	}
	return _richText;
}

#if WITH_EDITOR
void USliderPrefab::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	/*
		CheckPropertyOverrideStyle<TObjectPtr<UDataTable>>(PropertyChangedEvent, "overrideStyleTitle",
		                                                                 overrideStyleTitle,
		                                                                 bOverrideStyleTitle, [this]
		                                                                 {		                                                                 
			                                                                 title->SetStyle(nullptr, false);
		                                                                 },
		                                                                 [this]
		                                                                 {
			                                                                 title->SetStyle(
				                                                                 overrideStyleTitle, bOverrideStyleTitle);
		                                                                 });*/


	const FString _property = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetName() : "";

	if (_property == "minValue")
	{
		if (minValue > maxValue)
		{
			minValue = maxValue;
		}
	}
	if (_property == "maxValue")
	{
		if (maxValue < minValue)
		{
			maxValue = minValue;
		}
	}
}
#endif
