// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "TextPrefab.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UTextPrefab final : public UPrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URichTextBlock> textBlock;

	UPROPERTY(EditAnywhere, Category = "Custom", meta=(MultiLine=true))
	FString textFormat = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom", meta=(MultiLine=true))
	FText text = TEXTSTR("PLACEHOLDER");

	/** Number of char to clamp if text is too long. Set to -1 to disable)*/
	UPROPERTY(EditAnywhere, Category = "Custom", meta=(UIMin = -1, ClampMin = -1))
	int numberCharClamp = -1;

	//UPROPERTY(EditAnywhere, Category = "Custom|Style", meta=(EditCondition="bOverrideStyle"))
	//TObjectPtr<UDataTable> styleSet;

	UPROPERTY(EditAnywhere, Category = "Custom")
	bool wrapText = false;

	/** Can override the format text */
	UPROPERTY(EditAnywhere, Category = "Custom")
	bool isOverrideFormat = false;

public:
	FORCEINLINE TObjectPtr<URichTextBlock>& GetText() { return textBlock; }
	FORCEINLINE int GetClampLength() const { return numberCharClamp; }
	FORCEINLINE void SetText(const FString& _text) const { textBlock->SetText(FText::FromString(_text)); }

	//void SetStyle(UDataTable* _style, const bool& _active);

private:
	virtual void NativePreConstruct() override;

	//void LoadStyle() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
