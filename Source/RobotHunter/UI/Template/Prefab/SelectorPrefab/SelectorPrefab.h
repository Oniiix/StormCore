// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "SelectorPrefab.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API USelectorPrefab final : public UOptionablePrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftSelection);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightSelection);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnRightSelection onRightSelection;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLeftSelection onLeftSelection;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> title;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> leftSelector;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> rightSelector;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> selectionText;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<FText> listSelection;

	/*UPROPERTY(EditAnywhere, Category = "Custom")
	FString textFormat = "<normal>{0}</>";
	
	UPROPERTY(EditAnywhere, Category = "Custom")
	FString textChoiceFormat = "<normal>{0}</>";
	*/
	UPROPERTY(EditAnywhere, Category = "Custom")
	FString text = "EXAMPLE";

	UPROPERTY(EditAnywhere, Category = "Custom")
	bool hasTitle = true;

	UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	bool bOverrideStyleButtons = false;
	UPROPERTY(EditAnywhere, Category = "Custom|Button|Style", meta=(EditCondition="bOverrideStyleButtons"))
	TObjectPtr<UButtonPrefabOverrideStyle> overrideStyleButtons;

	/*UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	bool bOverrideStyleTitle = false;
	UPROPERTY(EditAnywhere, Category = "Custom|Text|Style", meta=(EditCondition="bOverrideStyleTitle"))
	TObjectPtr<UDataTable> overrideStyleTitle;*/

	//UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	//bool bOverrideStyleTextSelection = false;

	//UPROPERTY(EditAnywhere, Category = "Custom|Text|Style", meta=(EditCondition="bOverrideStyleTextSelection"))
	//TObjectPtr<UDataTable> overrideStyleTextSelections;

public:
	FORCEINLINE FOnLeftSelection& OnLeftSelection() { return onLeftSelection; }
	FORCEINLINE FOnRightSelection& OnRightSelection() { return onRightSelection; }

	virtual void SetOptionParameter(const FString& _name, const TArray<FString>& _list) override;

private:
	UFUNCTION()
	FORCEINLINE void SetRightSelection();
	UFUNCTION()
	FORCEINLINE void SetLeftSelection();
	UFUNCTION()
	FORCEINLINE void Callback_OnLeftSelection(){ onLeftSelection.Broadcast(); }

	UFUNCTION()
	FORCEINLINE void Callback_OnRightSelection() { onRightSelection.Broadcast(); }


	virtual void NativePreConstruct() override;
	/*virtual void Init() override;
	void CustomRegisterEvents();*/
	virtual void RegisterEvents() override;
	virtual void SetCurrentIndex(const int& _index) override;
	virtual void SetCurrentIndexWithoutEvent(const int& _index) override;

	void LoadStyleButtons() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};