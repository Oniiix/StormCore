// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectorPrefab.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"


void USelectorPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!(title && selectionText && leftSelector && rightSelector))
	{
		return;
	}

	SetEnableWidget<UTextPrefab>(title, hasTitle);

	SET_TEXT_STR(title->GetText(), text);
	const FString _text = listSelection.Num() > 0 ? listSelection[0].ToString() : "No Selection Text";
	//SET_TEXT_STR(selectionText->GetText(),FORMAT(textChoiceFormat, listSelection listSelection[0].ToString()));
	//SET_TEXT_STR(selectionText->GetText(),FORMAT(textChoiceFormat, listSelection[0].ToString()));

	//title->SetStyle(overrideStyleTitle, bOverrideStyleTitle);
	//selectionText->SetStyle(overrideStyleTextSelections, bOverrideStyleTextSelection);

	if (bOverrideStyleButtons)
	{
		LoadStyleButtons();
	}
}

void USelectorPrefab::RegisterEvents()
{
	Super::RegisterEvents();

	onLeftSelection.AddUniqueDynamic(this, &USelectorPrefab::SetLeftSelection);
	onRightSelection.AddUniqueDynamic(this, &USelectorPrefab::SetRightSelection);

	BIND_ACTION(leftSelector, OnClick, USelectorPrefab, Callback_OnLeftSelection);
	BIND_ACTION(rightSelector, OnClick, USelectorPrefab, Callback_OnRightSelection);
}

void USelectorPrefab::SetCurrentIndex(const int& _index)
{
	Super::SetCurrentIndex(_index);
	if (listSelection.IsEmpty())
	{
		LOG_ERROR(true, "List selection is null");
		return;
	}
	SET_TEXT_STR(selectionText->GetText(), listSelection[index].ToString());
	if (!data)
	{
		return;
	}

	Callback_OnChangeIndex(index);
	Callback_OnChangeDetected();
}

void USelectorPrefab::SetCurrentIndexWithoutEvent(const int& _index)
{
	Super::SetCurrentIndexWithoutEvent(_index);
	if (_index >= listSelection.Num() || _index < 0)
	{
		return;
	}
	SET_TEXT_STR(selectionText->GetText(), listSelection[index].ToString());
}

void USelectorPrefab::LoadStyleButtons() const
{
	leftSelector->SetStyle(overrideStyleButtons->GetStyle());
	rightSelector->SetStyle(overrideStyleButtons->GetStyle());
}

void USelectorPrefab::SetLeftSelection()
{
	const int _size = listSelection.Num() - 1;
	if (_size == 0)
	{
		return;
	}
	SetCurrentIndex(index - 1 < 0 ? _size : index - 1);
}


void USelectorPrefab::SetOptionParameter(const FString& _name, const TArray<FString>& _list)
{
	text = _name;
	SET_TEXT_STR(title->GetText(), text);

	for (const FString& List : _list)
	{
		listSelection.Add(TEXTSTR(List));
	}


	if (_list.Num() > 0)
	{
		if (index >= 0 || index <= _list.Num() - 1)
		{
			SET_TEXT_STR(selectionText->GetText(), listSelection[index].ToString());
		}
		else
		{
			SET_TEXT_STR(selectionText->GetText(), listSelection[0].ToString());
		}
	}
}

void USelectorPrefab::SetRightSelection()
{
	const int _size = listSelection.Num() - 1;
	if (_size == 0)
	{
		return;
	}

	SetCurrentIndex(index + 1 > _size ? 0 : index + 1);
}

#if WITH_EDITOR
void USelectorPrefab::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);


	CheckPropertyOverrideStyle<TObjectPtr<UButtonPrefabOverrideStyle>>(PropertyChangedEvent, "overrideStyleButtons",
	                                                                   overrideStyleButtons,
	                                                                   bOverrideStyleButtons,
	                                                                   [this] { overrideStyleButtons = nullptr; },
	                                                                   [this]
	                                                                   {
		                                                                   LoadStyleButtons();
	                                                                   });

	/*CheckPropertyOverrideStyle<TObjectPtr<UDataTable>>(PropertyChangedEvent, "overrideStyleTitle",
	                                                                 overrideStyleTitle,
	                                                                 bOverrideStyleTitle, [this]
	                                                                 {
		                                                                 overrideStyleButtons = nullptr;
		                                                                 title->SetStyle(nullptr, false);
	                                                                 },
	                                                                 [this]
	                                                                 {
		                                                                 title->SetStyle(
			                                                                 overrideStyleTitle, bOverrideStyleTitle);
	                                                                 });

	CheckPropertyOverrideStyle<TObjectPtr<UDataTable>>(PropertyChangedEvent,
	                                                                 "overrideStyleTextSelections",
	                                                                 overrideStyleTextSelections,
	                                                                 bOverrideStyleTextSelection, [this]
	                                                                 {
		                                                                 overrideStyleTextSelections = nullptr;
		                                                                 selectionText->SetStyle(nullptr, false);
	                                                                 },
	                                                                 [this]
	                                                                 {
		                                                                 selectionText->SetStyle(
			                                                                 overrideStyleTextSelections,
			                                                                 bOverrideStyleTextSelection);
	                                                                 });*/
}
#endif
