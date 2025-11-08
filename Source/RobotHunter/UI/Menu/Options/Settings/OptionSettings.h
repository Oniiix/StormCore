// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Spacer.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "RobotHunter/UI/Menu/Options/Prefab/ButtonCategoryPrefab.h"
#include "RobotHunter/Utils/ENUM_Option.h"
#include "OptionSettings.generated.h"

USTRUCT()
struct FOptionCategory
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, meta = (EditCondition = "category != ECategoryOption::None", EditConditionHides))
	TArray<TSubclassOf<UOptionData>> optionsReference;

	UPROPERTY()
	TArray<TObjectPtr<UOptionData>> options;	
};

USTRUCT()
struct FOption
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FText name;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "category != ECategoryOption::None", EditConditionHides))
	TArray<FOptionCategory> options;
	
	UPROPERTY(EditAnywhere)
	bool bCanReset = true;
};

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UOptionSettings final : public UDataAsset
{
	GENERATED_BODY()

	//Button to be created for category buttons
	UPROPERTY(EditAnywhere, Category = "Category|Reference")
	TSubclassOf<UButtonCategoryPrefab> buttonPrefabReference;

	//The space between each sub category in category
	UPROPERTY(EditAnywhere, Category = "Category|Options")
	float spaceBetweenCategory;
	//Space between element created
	UPROPERTY(EditAnywhere, Category = "Category|Options")
	int paddingBetweenElement = 10;

	//Button to be created for category buttons
	UPROPERTY(EditAnywhere, Category = "Category|Reference")
	TSubclassOf<USpacer> spacerReference;

	//The different name by category key
	UPROPERTY(EditAnywhere, Category = "Category")
	TMap<ECategoryOption, FOption> categoryNameByEnum;

	UPROPERTY()
	TObjectPtr<UGameUserSettings> GUS;

	UPROPERTY(EditAnywhere, Category ="Custom")
	FString textFormat = "<normal>{0}</>";

public:
	FORCEINLINE float GetSpaceBetweenCategory() const { return spaceBetweenCategory; }
	FORCEINLINE FOption& GetOptionByCategory(ECategoryOption _category) { return categoryNameByEnum[_category]; }
	FORCEINLINE TSubclassOf<UButtonCategoryPrefab> GetButtonReference() const { return buttonPrefabReference; }
	FORCEINLINE TSubclassOf<USpacer> GetSpacerReference() const { return spacerReference; }
	FORCEINLINE TMap<ECategoryOption, FOption> GetCategoryNameByEnum() const { return categoryNameByEnum; }

	FORCEINLINE int GetPaddingBetweenElement() const { return paddingBetweenElement; }
	/**
	 * Get all the category name for all key
	 * @return Array of category name
	 */
	TArray<FString> GetCategoryName() const;

	/**
	* Get all category key
	* @return Array of category key
	*/
	TArray<ECategoryOption> GetCategory() const;

	/**
	 * Load all options and set a parent
	 * @param _parent Parent frame
	 * @param _gus GameUserSettings pointer
	 * @param _world
	 * @param _settings
	 */
	void LoadOptions(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                 const TObjectPtr<UWorld>& _world, const
	                 TObjectPtr<USettingsSave>& _settings);

	/**
	 * Reset option category
	 */
	void ResetOptions(const ECategoryOption _category);
};
