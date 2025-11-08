// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "ButtonSaveTemplate.generated.h"

class USaveManagerSubsystem;
class UHostSettings;
class UHostMenu;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UButtonSaveTemplate final : public UButtonPrefab
{
	GENERATED_BODY()

	UPROPERTY()
	UHostMenu* hostMenu;

	UPROPERTY()
	int indexSave = -1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textHour;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> saveNumber;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> sectorUnlock;*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> deleteBtn;

	/*UPROPERTY()
	UHostSettings* settings;*/

	UPROPERTY()
	USaveManagerSubsystem* saveManager;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText saveDateFormat = TEXTSTR("<titre>{0}{1}, {2}</>");
	UPROPERTY(EditAnywhere, Category = "Custom")
	FText hourDateFormat = TEXTSTR("<italic>{0}:{1}:{2}</>");
	UPROPERTY(EditAnywhere, Category = "Custom")
	FText saveNumberFormat = TEXTSTR("<titre>Save {0}</>");

public:
	/**
	 * Get the index of save
	 * @return int
	 */
	FORCEINLINE int GetIndexSave() const { return indexSave; }

	/**
	 * Setup the button of save
	 * @param _index Index of save
	 * @param _hostMenu UHostMenu*
	 * @param _settings const UHostSettings*
	 */
	void Setup(const int _index, UHostMenu* _hostMenu/*, UHostSettings* _settings*/);
	/**
	 * Refresh color of button if selected
	 */
	void RefreshColor();

private:
	virtual void RegisterEvents() override;
	/**
	 * Delete this save game
	 */
	UFUNCTION()
	void DeleteSave();
	/**
	 * Select this save game
	 */
	UFUNCTION()
	void SelectSave();
};
