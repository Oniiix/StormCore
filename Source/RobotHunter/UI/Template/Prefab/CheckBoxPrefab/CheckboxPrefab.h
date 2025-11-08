// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckboxPrefabOverrideStyle.h"
#include "Components/CheckBox.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "CheckboxPrefab.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UCheckboxPrefab final : public UOptionablePrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckChange, bool, checked);


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> titleCheckbox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> checkboxActive;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCheckBox> CheckBox;

	UPROPERTY(EditAnywhere, Category = "Custom|Style", meta=(EditCondition="bOverrideStyle"))
	TObjectPtr<UCheckboxPrefabOverrideStyle> overrideStyle;

	//Check this if you want to show title in checkbox
	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	bool bShowTitle = true;

	/*UPROPERTY(EditAnywhere, Category = "Custom|Text|Format", meta = (EditCondition = "bShowTitle", EditConditionHides))
	FString textFormat = "<normal>{0}</>";
	
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Format", meta = (EditCondition = "bShowTitle", EditConditionHides))
	FString choiceFormat = "<normal>{0}</>";
	
	*/
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Format", meta = (EditCondition = "bShowTitle", EditConditionHides))
	FString text = "title example";

	//Check this if you want to show status of checkbox
	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	bool bShowStatus = true;

	//Set the default status of checkbox
	UPROPERTY(EditAnywhere, Category = "Custom|Check")
	bool bDefaultChecked = false;

	/** The text of state of checkbox */
	UPROPERTY(EditAnywhere, Category = "Custom|Text", EditFixedSize,
		meta = (EditCondition = "bShowStatus", EditConditionHides))
	TArray<FString> checkboxTexts = {"Enabled", "Disabled"};

public:
	UPROPERTY(BlueprintAssignable)
	FOnCheckChange onCheckChange;

	UFUNCTION()
	bool IsChecked() const;

private:
	UFUNCTION(BlueprintCallable)
	void UpdateCheckbox(bool bIsChecked);

	virtual void NativePreConstruct() override;
	virtual void RegisterEvents() override;
	void LoadStyle() const;

	virtual void SetOptionParameter(const FString& _name, const TArray<FString>& _list) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
