// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"
#include "RobotHunter/UI/Template/Prefab/InputFieldPrefab/InputFieldPrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "SliderPrefab.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API USliderPrefab final : public UOptionablePrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftSelection);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightSelection);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> title;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USlider> sliderWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UInputFieldPrefab> InputValue;

	UPROPERTY(EditAnywhere, Category = "Custom")
	int minValue;

	UPROPERTY(EditAnywhere, Category = "Custom")
	int maxValue;

	/*
	UPROPERTY(EditAnywhere, Category = "Custom")
	FString textFormat = "<normal>{0}</>";
*/
	UPROPERTY(EditAnywhere, Category = "Custom")
	FString text = "EXAMPLE";

	UPROPERTY(EditAnywhere, Category = "Custom")
	bool hasTitle = true;

	/*UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	bool bOverrideStyleTitle = false;
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Style", meta=(EditCondition="bOverrideStyleTitle"))
	TObjectPtr<UDataTable> overrideStyleTitle;*/

public:
	virtual void SetOptionParameter(const FString& _name, const TArray<FString>& _list) override;

private:
	virtual void NativePreConstruct() override;
	virtual void RegisterEvents() override;
	virtual void SetCurrentIndex(const int& _index) override;
	virtual void SetCurrentIndexWithoutEvent(const int& _index) override;

	UFUNCTION()
	void ChangeValue(float _value);
	UFUNCTION()
	void ChangeValueByText(const FString& _value);

	FString ExtractNumberFromRichText(const FString& _richText) const;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
