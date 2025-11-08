// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollboxPrefab.h"

void UScrollboxPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (bOverrideStyle && overrideStyle)
	{
		LoadStyle();
	}
}

void UScrollboxPrefab::LoadStyle() const
{
	if (!scrollbox)
	{
		return;
	}
	scrollbox->SetWidgetStyle(overrideStyle->GetStyleScroll());
	scrollbox->SetWidgetBarStyle(overrideStyle->GetStyleBar());
}
#if WITH_EDITOR
void UScrollboxPrefab::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckPropertyOverrideStyle<TObjectPtr<UScrollboxPrefabOverrideStyle>>(
		PropertyChangedEvent, "overrideStyle", overrideStyle,
		bOverrideStyle, [this] { overrideStyle = nullptr; },
		[this] { LoadStyle(); });
}
#endif
