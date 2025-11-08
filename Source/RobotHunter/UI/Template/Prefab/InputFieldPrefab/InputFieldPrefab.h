// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputFieldPrefabOverrideStyle.h"
#include "Components/EditableTextBox.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "InputFieldPrefab.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UInputFieldPrefab final : public UPrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextChange, const FString&, _text);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnTextChange onTextChange;


	/*UPROPERTY(EditAnywhere, Category = "Custom")
	FString textFormat = "<normal>{0}</>";
		
	UPROPERTY(EditAnywhere, Category = "Custom")
	FText text = TEXTSTR("Placeholder");*/

	UPROPERTY(EditAnywhere, Category = "Custom|Style", meta=(EditCondition="bOverrideStyle"))
	TObjectPtr<UInputFieldPrefabOverrideStyle> overrideStyle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UEditableTextBox> inputField;

public:
	FORCEINLINE FOnTextChange& OnTextChange() { return onTextChange; }
	UFUNCTION(Blueprintable)
	FORCEINLINE FText GetText() const { return inputField->GetText(); }

	FORCEINLINE TObjectPtr<UEditableTextBox> GetInputField() const { return inputField; }

private:
	UFUNCTION()
	void TextChange(const FText& Text);
	virtual void NativePreConstruct() override;
	virtual void RegisterEvents() override;
	void LoadStyle() const;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
