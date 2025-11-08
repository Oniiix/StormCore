// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define SAVE_MANAGER_USE_LOG false

#define SETTINGS_SAVE_FILE "Settings/settings_data"
#define SLOT_GAME_SAVE_FILE FString("RH_Savegame_{0}")
#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/MainMenu/Host/ButtonSaveTemplate.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManagerSubsystem.generated.h"

class USettingsSave;
class UGameplaySave;
class UCustomSave;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USaveManagerSubsystem final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<int, UGameplaySave*> saves;

	UPROPERTY()
	TObjectPtr<USettingsSave> settings;

	short slotSaveLoaded = -1;

public:
#pragma region game save
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/**
	 * Set selected index of save
	 * @param IndexSave int
	 */
	FORCEINLINE void SetSelectedSave(int IndexSave) { slotSaveLoaded = IndexSave; }
	/**
	 * Get the current save index
	 * @return int
	 */
	FORCEINLINE int GetIndexOfSave() const { return slotSaveLoaded; }

	/**
	 * Get the number of save
	 * @return int
	 */
	FORCEINLINE int GetNumberSave() const
	{
		if (saves.IsEmpty()) return 0;
		return saves.Num() - 1;
	}

	/**
	 * Create save game
	 */
	void CreateSave();
	/**
	 * Save at slot
	 * @param _index index of slot
	 * @param _saveTime 
	 */
	void Save(int _index, bool _saveTime = true) const;

	/**
	 * Save the curernt savegame
	 * @param _saveTime save time 
	 */
	void SaveCurrent(bool _saveTime = false) const;
	/**
	 * Delete save game at slot and return true if success
	 * @param IndexSave int
	 * @return bool
	 */
	bool DeleteSave(int IndexSave);
	bool DeleteSaveWithoutReorder(int IndexSave);
	/**
	 * Get all saves
	 * @return TArray<UGameSave*>
	 */
	TArray<UGameplaySave*> GetSaves() const;
	/**
	 * Get the current save
	 * @return UGameSave*
	 */
	UFUNCTION(BlueprintPure)
	UGameplaySave* GetCurrentSave() const;
	/**
	 * Get the save at index
	 * @param IndexSave int
	 * @return UGameSave*
	 */
	UGameplaySave* GetSave(int IndexSave) const;

private:
	/**
	 * Load all saves
	 */
	void LoadSaves();
	/**
	 * Reorder save file and return if success
	 * @param index int
	 * @return bool
	 */
	bool ReorderSaveFromIndex(const int& index);
#pragma endregion game save

#pragma region save settings

public:
	UFUNCTION(BlueprintCallable, Category = "Custom|Save|Settings")
	USettingsSave* GetSaveSettings() const;
	void LoadSettings();
	void CreateSettings();
	UFUNCTION(BlueprintCallable, Category = "Custom|Save|Settings") void SaveSettings() const;
#pragma endregion save settings
};
