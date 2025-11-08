// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Pause/PauseFrame.h"
#include "RobotHunter/UI/Template/Prefab/ScrollBoxPrefab/ScrollboxPrefab.h"
#include "RobotHunter/UI/Template/Prefab/SeparatorPrefab/SeparatorPrefab.h"
#include "Settings/OptionSettings.h"
#include "OptionFrame.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UOptionFrame final : public UPauseFrame
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValueChange);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCategoryChange);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBack);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDirectBack);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetConfirmation);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnCategoryChange onCategoryChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnBack onBack;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnResetConfirmation onResetConfirmation;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnDirectBack onDirectBack;

	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	TObjectPtr<UOptionSettings> optionsSettings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollboxPrefab> ScrollboxCategory;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollboxPrefab> optionParameter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> applyParameter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> backBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> descriptionOption;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> resetBtn;

	UPROPERTY()
	ECategoryOption currentCategory = ECategoryOption::None;
	ECategoryOption oldCategory = ECategoryOption::None;

	UPROPERTY()
	TObjectPtr<UGameUserSettings> gus;

	bool hasModification = false;
	bool first = false;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	int categoryToShow = -1;

	UPROPERTY()
	TObjectPtr<UCustomGameInstance> instance;

public:
	FORCEINLINE TObjectPtr<UOptionSettings> GetSettings() const { return optionsSettings; }
	FORCEINLINE TObjectPtr<UScrollboxPrefab> GetOptionParameter() const { return optionParameter; }

	virtual void Init() override;
	virtual void Refresh() override;


	void AddChildParameter(const TObjectPtr<UOptionablePrefab>& _parameter) const;
	void AddCategory(const TSubclassOf<USeparatorPrefab>& _prefab, const FString& _name);
	UFUNCTION()
	void ShowCategory(const int _category);
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ConfirmShowCategory(int _cat);

private:
	UFUNCTION()
	void ShowValueChangedDetected(const UOptionablePrefab* _prefab);
	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (AllowPrivateAccess))
	void ApplyAllParameter();
	UFUNCTION()
	void ShowDescriptionOption(const FString& Msg);
	UFUNCTION()
	void Back();
	UFUNCTION(BlueprintCallable, Category = "Custom", meta=(AllowPrivateAccess))
	void ConfirmBack();
	UFUNCTION()
	void Reset();
	UFUNCTION(BlueprintCallable, Category = "Custom", meta=(AllowPrivateAccess))
	void ConfirmReset();

	virtual void RegisterEvents() override;

	void SetupPrefab(const TObjectPtr<UOptionData>& OptionData, const TObjectPtr<UOptionablePrefab>& _prefab,
	                 bool _canBeEdited);
	void ChangePresetOptionByOtherOption(const TObjectPtr<UOptionData>& _dataPrefab) const;

	/**
	 * Change all option if preset changeed
	 * @param _dataPrefab OptionData given
	 * @return True if preset has been change correctly
	 */
	void ChangePresetOption(const TObjectPtr<UOptionData>& _dataPrefab) const;


public:
	FORCEINLINE TObjectPtr<UButtonPrefab> GetApplyButton() const { return applyParameter; }
	TArray<TObjectPtr<UOptionablePrefab>> GetOptionsWidget() const;
};
