// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicsOptionBase.h"

TArray<FString> UGraphicsOptionBase::GetChoices() const
{
	TArray<FString> choices;
	for (const TTuple<EGraphicsOptionValue, FText>& Value : OptionsName)
	{
		FString _value = Value.Value.ToString();
		choices.Add(FORMAT(choiceFormat, _value));
	}
	return choices;
}

void UGraphicsOptionBase::ResetToDefault()
{
	TArray<EGraphicsOptionValue> keys;
	OptionsName.GetKeys(keys);

	currentIndex = keys.IndexOfByKey(defaultOption);
	Super::ResetToDefault();
}

#if WITH_EDITOR

void UGraphicsOptionBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName.ToString().ToLower() == "defaultOption")
	{
		SetDefaultValue(static_cast<int>(defaultOption));
	}
}
#endif
