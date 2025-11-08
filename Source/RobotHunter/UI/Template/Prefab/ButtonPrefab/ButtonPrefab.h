// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonPrefabOverrideStyle.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"

#include "ButtonPrefab.generated.h"


USTRUCT()
struct FBaseStyleButton
{
	GENERATED_BODY()

	FButtonStyle style;
	FLinearColor backgroundColor;
	FLinearColor colorAndOpacity;

	FBaseStyleButton() = default;

	FBaseStyleButton(const FButtonStyle& Style, const FLinearColor& BackgroundColor,
	                 const FLinearColor& ColorAndOpacity)
		: style(Style),
		  backgroundColor(BackgroundColor),
		  colorAndOpacity(ColorAndOpacity)
	{
	}
};

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UButtonPrefab : public UPrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClick);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickWithoutSound);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHover);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressed);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleased);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisabled);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnabled);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnHover);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelected);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnSelected);

protected:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnEnabled Custom_OnEnabled;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnDisabled Custom_OnDisabled;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnReleased Custom_OnReleased;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnPressed Custom_OnPressed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHover Custom_OnHover;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUnHover Custom_OnUnHover;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnClick Custom_OnClick;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnClickWithoutSound Custom_OnClickWithoutSound;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnSelected Custom_OnSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUnSelected Custom_OnUnSelected;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton> buttonBase;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textBase;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UScaleBox> SB;

	/** Text foramted to show on button */
	UPROPERTY(EditAnywhere, Category = "Custom|Text", meta=(MultiLine=true))
	FString textFormat = "<normal>{0}</>";

	/** Text to show on button */
	UPROPERTY(EditAnywhere, Category = "Custom|Text", meta=(MultiLine=true))
	FText text = TEXTSTR("Button Example");

	/** Padding of scaleBox in button */
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Settings")
	FMargin paddingText = FMargin(4, 2);

	/** Alignment horizontal of text in button */
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Settings")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = HAlign_Fill;
	/** Alignment vertical of text in button */
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Settings")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment = VAlign_Fill;

	//UPROPERTY(EditAnywhere, Category = "Custom|Text|Style", meta = (EditCondition="bOverrideStyle"))
	//TObjectPtr<UDataTable> textOverrideStyle;

	UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	bool bOverrideStyleButton = false;
	UPROPERTY(EditAnywhere, Category = "Custom|Style", meta = (EditCondition="bOverrideStyleButton"))
	TObjectPtr<UButtonPrefabOverrideStyle> buttonOverrideStyle;


	UPROPERTY(EditAnywhere, Category = "Custom|Style")
	bool bCanBeSelected = false;

	UPROPERTY(EditAnywhere, Category = "Custom|Style", meta = (EditCondition="bCanBeSelected", EditConditionHides))
	TObjectPtr<UButtonPrefabOverrideStyle> buttonOverrideStyleSelected;

	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	bool isSelected = false;

	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	bool isFocused = false;


	UPROPERTY()
	FBaseStyleButton baseStyle;

public:
	FORCEINLINE FOnClick& OnClick() { return Custom_OnClick; }
	FORCEINLINE FOnHover& OnHover() { return Custom_OnHover; }
	FORCEINLINE FOnPressed& OnPressed() { return Custom_OnPressed; }
	FORCEINLINE FOnReleased& OnReleased() { return Custom_OnReleased; }
	FORCEINLINE FOnDisabled& OnDisabled() { return Custom_OnDisabled; }
	FORCEINLINE FOnEnabled& OnEnabled() { return Custom_OnEnabled; }
	FORCEINLINE FOnUnHover& OnUnHover() { return Custom_OnUnHover; }
	FORCEINLINE FOnSelected& OnSelected() { return Custom_OnSelected; }
	FORCEINLINE FOnUnSelected& OnUnSelected() { return Custom_OnUnSelected; }

	FORCEINLINE TObjectPtr<UTextPrefab> GetText() const { return textBase; }
	FORCEINLINE TObjectPtr<UButton> GetButtonBase() const { return buttonBase; }
	UFUNCTION(Blueprintable, BlueprintCallable, Category = "Custom")
	FORCEINLINE bool IsSelectedButton() const { return isSelected; }

	FORCEINLINE void SetSelected(const bool _selected);

	/**
	 * Callback when button clicked
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Button")
	FORCEINLINE void Callback_OnClick()
	{
		Callback_OnClick(true);
	}

	FORCEINLINE void Callback_OnClick(const bool playSound)
	{
		if (!isSelected)
		{
			if (playSound)
				Custom_OnClick.Broadcast();
			else
				Custom_OnClickWithoutSound.Broadcast();
			if (bCanBeSelected)
			{
				Callback_OnSelected();
			}
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Custom|Button")
	FORCEINLINE void Callback_OnUnSelected()
	{
		if (isSelected)
		{
			Custom_OnUnSelected.Broadcast();
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Custom|Button")
	FORCEINLINE void Callback_OnSelected(const bool playSound = false)
	{
		if (!isSelected)
		{
			Custom_OnSelected.Broadcast();
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Custom|Button")
	void Callback_OnHover();

	UFUNCTION(BlueprintCallable, Category = "Custom|Button")
	FORCEINLINE void Callback_OnUnHover()
	{
		if (!isSelected)
		{
			Custom_OnUnHover.Broadcast();
			isFocused = false;
		}
	}

	bool CanKeepSelected();

	/**
	 * Se the style of button
	 * @param _style const FButtonStyle&
	 */
	void SetStyle(const FButtonStyle& _style) const;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void RegisterEvents() override;

private:
	UFUNCTION()
	void UnSelectedButton();
	UFUNCTION()
	void SelectedButton();
	/**
	 * Enable button
	 */
	UFUNCTION()
	void DisableButton();
	/**
	 * Disable button
	 */
	UFUNCTION()
	void EnableButton();

	/**
	 * Callback when button hovered
	 */


	/**
	 * Callback when button pressed
	 */
	UFUNCTION()
	FORCEINLINE void Callback_OnPressed()
	{
		if (!isSelected)
		{
			Custom_OnPressed.Broadcast();
		}
	}

	/**
	 * Callback when button released
	 */
	UFUNCTION()
	FORCEINLINE void Callback_OnReleased()
	{
		if (!isSelected)
		{
			Custom_OnReleased.Broadcast();
		}
	}

	/**
	 * Callback when button disabled
	 */
	UFUNCTION()
	FORCEINLINE void Callback_OnDisabled() { Custom_OnDisabled.Broadcast(); }

	/**
	 * Callback when button enabled
	 */
	UFUNCTION()
	FORCEINLINE void Callback_OnEnabled() { Custom_OnEnabled.Broadcast(); }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


	void LoadStyleButton() const;
	void LoadStyleText() const;
};
