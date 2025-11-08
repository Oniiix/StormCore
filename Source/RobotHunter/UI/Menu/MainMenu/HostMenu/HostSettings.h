// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/BaseSettings.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/Utils/DebugUtils.h"

#include "HostSettings.generated.h"

class UButtonSaveTemplate;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UHostSettings final : public UBaseSettings
{
	GENERATED_BODY()

	/** This is template button of save game */
	UPROPERTY(EditAnywhere, Category = "Template|Template")
	TSubclassOf<UButtonSaveTemplate> templateButtonSave;

	/** This is template button of create save game */
	UPROPERTY(EditAnywhere, Category = "Template|Template")
	TSubclassOf<UButtonPrefab> templateButtonCreateSave;

	/** Set the differents name of accessibility state (DO NOT CHANGE THE KEY) */
	UPROPERTY(EditAnywhere, Category = "Server")
	TMap<TEnumAsByte<EOnlineAccessibilityState>, FText> stateNames = {
		{Public, TEXTSTR("Public")},
		{OnlyFriends, TEXTSTR("Friend")},
		{Private, TEXTSTR("Private")}
	};

	/** Default name showed in field */
	UPROPERTY(EditAnywhere, Category = "Server")
	FText defaultServerName = TEXTSTR("{0}'s Room");

	/**	 Text template for title button of save	 */
	UPROPERTY(EditAnywhere, Category = "Template|Text")
	FText saveText = TEXTSTR("Save {0}");

	/** Money text template */
	UPROPERTY(EditAnywhere, Category = "Template|Text")
	FText moneyText = TEXTSTR("Money: {0}");

	/** sector text template */
	UPROPERTY(EditAnywhere, Category = "Template|Text")
	FText sectorText = TEXTSTR("Sector: {0}");

	/** Brush of button if selected */

	UPROPERTY(EditAnywhere, Category = "Template|Style")
	TObjectPtr<UButtonPrefabOverrideStyle> saveSelected;

	/** Brush of button if not selected */

	UPROPERTY(EditAnywhere, Category = "Template|Style")
	TObjectPtr<UButtonPrefabOverrideStyle> saveBase;

public:
	/**
	 * Get the state name to set in UI
	 * @param state EOnlineAccessibilityState
	 * @return FText
	 */
	FORCEINLINE FText GetStateName(EOnlineAccessibilityState state) const { return stateNames[state]; }

	/**
	 * Get the template of save button
	 * @return TSubclassOf<UButtonSaveTemplate>
	 */
	FORCEINLINE TSubclassOf<UButtonSaveTemplate> GetTemplateSave() const { return templateButtonSave; }
	/**
	 * Get the template of create save button
	 * @return TSubclassOf<UButtonCreateSaveTemplate>
	 */
	FORCEINLINE TSubclassOf<UButtonPrefab> GetTemplateCreateSave() const
	{
		return templateButtonCreateSave;
	}

	/**
	 * Get the default name serveur
	 * @return FText
	 */
	FORCEINLINE FText GetDefaultServerName() const { return defaultServerName; }

	/**
	 * Get the formated saved text
	 * @param _index int
	 * @return FString 
	 */
	FORCEINLINE FString GetFormatedSaveText(int _index) const { return FORMAT(saveText.ToString(), _index); }
	/**
	 * Get the formated money text
	 * @param _money int
	 * @return FString 
	 */
	FORCEINLINE FString GetFormatedMoneyText(int _money) const { return FORMAT(moneyText.ToString(), _money); }
	/**
	 * Get the formated sector text
	 * @param _sector int
	 * @return FString
	 */
	FORCEINLINE FString GetFormatedSectorText(int _sector) const { return FORMAT(sectorText.ToString(), _sector); }

	FORCEINLINE TObjectPtr<UButtonPrefabOverrideStyle> GetSelectedSave() const { return saveSelected; }
	FORCEINLINE TObjectPtr<UButtonPrefabOverrideStyle> GetNormalSave() const { return saveBase; }
};
