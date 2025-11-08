// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManagerSubsystem.h"

#include "GameplaySave.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SettingsSave.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "GameFramework/GameUserSettings.h"
#include "RobotHunter/QuestSystem/QuestSubsystem.h"
#include "RobotHunter/Game/CustomGameInstance.h"

void USaveManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UGameUserSettings* _settings = UGameUserSettings::GetGameUserSettings();
	_settings->ApplyResolutionSettings(false);

	LoadSettings();

	CALLBACK_AFTER_TIME(.1f, &USaveManagerSubsystem::LoadSaves, GetWorld());
}

#pragma region game save
TArray<UGameplaySave*> USaveManagerSubsystem::GetSaves() const
{
	TArray<UGameplaySave*> result;
	for (const TTuple<int, UGameplaySave*>& _save : saves)
	{
		result.Add(_save.Value);
	}
	return result;
}

UGameplaySave* USaveManagerSubsystem::GetCurrentSave() const
{
	if (slotSaveLoaded == -1 || saves.Num() == 0)
		return nullptr;
	return saves[slotSaveLoaded];
}

UGameplaySave* USaveManagerSubsystem::GetSave(int IndexSave) const
{
	CHECK_OUT_OF_BOUNDS(IndexSave, 0, saves.Num(), "Index is invalid", return nullptr);
	return saves[IndexSave];
}

void USaveManagerSubsystem::CreateSave()
{
	const int lastSave = saves.Num();
	if (UGameplaySave* SaveGameInstance = Cast<UGameplaySave>(
		UGameplayStatics::CreateSaveGameObject(UGameplaySave::StaticClass())))
	{
		//SaveGameInstance->SavePlayer(nullptr);
		SaveGameInstance->SaveTime();
		//SaveGameInstance->SaveFriend(nullptr);
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, FORMAT(SLOT_GAME_SAVE_FILE, lastSave), 0))
		{
			saves.Add(lastSave, SaveGameInstance);
			SetSelectedSave(lastSave);
		}
	}
}

void USaveManagerSubsystem::Save(const int _index, const bool _saveTime) const
{
	/*if (UGameplaySave* SaveGameInstance = Cast<UGameplaySave>(
		UGameplayStatics::CreateSaveGameObject(UGameplaySave::StaticClass())))*/

	if (UGameplaySave* _save = GetSave(_index))
	{
		if (_saveTime)
			_save->SaveTime();
		//SaveGameInstance->SavePlayer(nullptr);
		//SaveGameInstance->SaveFriend(nullptr);
		if (UGameplayStatics::SaveGameToSlot(_save, FORMAT(SLOT_GAME_SAVE_FILE, _index), 0))
		{
			LOG(true, "Success save");
		}
	}
}

void USaveManagerSubsystem::SaveCurrent(const bool _saveTime) const
{
	/*if (UGameplaySave* SaveGameInstance = Cast<UGameplaySave>(
		UGameplayStatics::CreateSaveGameObject(UGameplaySave::StaticClass())))*/

	if (UGameplaySave* _save = GetCurrentSave())
	{
		if (_saveTime)
			_save->SaveTime();
		//SaveGameInstance->SavePlayer(nullptr);
		//SaveGameInstance->SaveFriend(nullptr);
		if (UGameplayStatics::SaveGameToSlot(_save, FORMAT(SLOT_GAME_SAVE_FILE, slotSaveLoaded), 0))
		{
			LOG(true, "Success save");
		}
	}
}

bool USaveManagerSubsystem::DeleteSave(int IndexSave)
{
	if (IndexSave == -1) return false;
	UGameplayStatics::DeleteGameInSlot(FORMAT(SLOT_GAME_SAVE_FILE, IndexSave), 0);
	slotSaveLoaded = -1;
	return ReorderSaveFromIndex(IndexSave);
}

void USaveManagerSubsystem::LoadSaves()
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(), "Load all saves from subsystem");
	LOG_COLOR(SAVE_MANAGER_USE_LOG, "Load all saves from subsystem", ORANGE);


	int index = 0;
	FString slotName = FORMAT(SLOT_GAME_SAVE_FILE, index);
	while (const TObjectPtr<UGameplaySave>& game = Cast<UGameplaySave>(UGameplayStatics::LoadGameFromSlot(slotName, 0)))
	{
		saves.Add(index, game);
		index++;
		slotName = FORMAT(SLOT_GAME_SAVE_FILE, index);
	}
}

bool USaveManagerSubsystem::ReorderSaveFromIndex(const int& index)
{
	saves.Remove(index);
	if (saves.IsEmpty()) return true;

	const int saveCount = saves.Num();
	TMap<int, UGameplaySave*> newSavesMap;


	for (int i = index + 1; i <= saveCount; i++)
	{
		UGameplaySave* save = *saves.Find(i);
		CHECK_NULL_WITH_RETURN(save, "Save is null", return false);

		if (!UGameplayStatics::DeleteGameInSlot(FORMAT(SLOT_GAME_SAVE_FILE, i), 0))
		{
			FString error = FORMAT(FString("Error during delete game at slot {0}"), i);
			LOG_COLOR(true, error, RED);
			return false;
		}
		if (!UGameplayStatics::SaveGameToSlot(save, FORMAT(SLOT_GAME_SAVE_FILE, i-1), 0))
		{
			FString error = FORMAT(FString("Error during save game at slot {0}"), i);
			LOG_COLOR(true, FORMAT(error, i), RED);
			return false;
		}
		newSavesMap.Add(i - 1, save);
	}

	for (int i = index + 1; i <= saveCount; i++)
		saves.Remove(i);

	for (const TTuple<int, UGameplaySave*>& NewSavesMap : newSavesMap)
		saves.Add(NewSavesMap.Key, NewSavesMap.Value);
	return true;
}
#pragma endregion game save

void USaveManagerSubsystem::LoadSettings()
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(), "Load all saved settings");
	if (UGameplayStatics::DoesSaveGameExist(SETTINGS_SAVE_FILE, 0))
	{
		if (USaveGame* _save = UGameplayStatics::LoadGameFromSlot(SETTINGS_SAVE_FILE, 0))
		{
			USettingsSave* _set = Cast<USettingsSave>(_save);
			settings = _set;
		}
	}
	else
		CreateSettings();
}

void USaveManagerSubsystem::CreateSettings()
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(), "Create save settings");
	if (USaveGame* _settings = UGameplayStatics::CreateSaveGameObject(USettingsSave::StaticClass()))
	{
		USettingsSave* _set = Cast<USettingsSave>(_settings);
		settings = _set;
		SaveSettings();
	}
}

USettingsSave* USaveManagerSubsystem::GetSaveSettings() const
{
	USaveGame* _saveGame = settings;
	if (!_saveGame)
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::SubSystem, GetClass(),
		                      "Failed to get settings");
	CHECK_NULL_WITH_RETURN(_saveGame, "Save game is null", return nullptr);
	USettingsSave* _save = Cast<USettingsSave>(_saveGame);
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(), "Trying cast save");
	if (!_save)
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::SubSystem, GetClass(),
		                      "Failed to cast settings");
	return _save;
}

void USaveManagerSubsystem::SaveSettings() const
{
	UGameplayStatics::SaveGameToSlot(settings, SETTINGS_SAVE_FILE, 0);
}
