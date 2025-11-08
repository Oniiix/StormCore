// Fill out your copyright notice in the Description page of Project Settings.


#include "TextPrefab.h"

#include "RobotHunter/Utils/DebugUtils.h"
/*
void UTextPrefab::SetStyle(UDataTable* _style, const bool& _active)
{
	bOverrideStyle = _active;
	styleSet = _style;
	if (bOverrideStyle && styleSet)
	{
		LoadStyle();
	}
}
*/

void UTextPrefab::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Prépare la chaîne de base à afficher
	FString baseString = text.ToString();
	if (numberCharClamp != -1 && baseString.Len() > numberCharClamp)
	{
		baseString = baseString.Mid(0, numberCharClamp) + "...";
	}

	// Applique le format ou non selon isOverrideFormat
	if (!isOverrideFormat)
	{
		SET_TEXT_STR(textBlock, FORMAT(textFormat, baseString));
	}
	else
	{
		SET_TEXT_STR(textBlock, baseString);
	}

	// Gestion du wrapping
	textBlock->SetAutoWrapText(wrapText);
	if (wrapText)
	{
		textBlock->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
		textBlock->SetWrapTextAt(0);
	}
}


#if WITH_EDITOR
void UTextPrefab::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	/*CheckPropertyOverrideStyle<TObjectPtr<UDataTable>>(PropertyChangedEvent, "overrideStyle",
	                                                                 styleSet,
	                                                                 bOverrideStyle,
	                                                                 [this] { styleSet = nullptr; },
	                                                                 [this] { LoadStyle(); });*/
}
#endif
/*
void UTextPrefab::LoadStyle() const
{
	if (!textBlock)
	{
		return;
	}
	textBlock->SetTextStyleSet(styleSet);
	
	/*textBlock->SetColorAndOpacity(overrideStyle->GetColorAndOpacity());
	textBlock->SetFont(overrideStyle->GetFont());
	textBlock->SetStrikeBrush(overrideStyle->GetStrikeBrush());
	textBlock->SetShadowOffset(overrideStyle->GetShadowOffset());
	textBlock->SetShadowColorAndOpacity(overrideStyle->GetShadowColorAndOpacity());
	textBlock->SetTextTransformPolicy(overrideStyle->GetTransformPolicy());
	textBlock->SetJustification(overrideStyle->GetJustification());
}
*/
