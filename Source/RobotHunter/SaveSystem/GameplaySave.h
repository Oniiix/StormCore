// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/QuestSystem/Asset/QuestAsset.h"
#include "GameplaySave.generated.h"

class AMainCharacter;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTutoSave
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool hasStartTuto = false;
};

USTRUCT(BlueprintType)
struct FMemberSave
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EMemberType> rightArmType = NONE;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EMemberType> leftArmType = NONE;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EMemberType> calfType = NONE;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EMemberType> thighType = NONE;
};


USTRUCT(BlueprintType)
struct FMeteoSave
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	int currentMeteoIndex = 0;
};

USTRUCT(BlueprintType)
struct FGameSave
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	int day = 0;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool isTutorial = false;
};

USTRUCT(BlueprintType)
struct FGameQuest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<int> IdQuests;
};

USTRUCT(BlueprintType)
struct FPlayerSave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	FDateTime date = FDateTime();
};

USTRUCT(BlueprintType)
struct FBonusSave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<FBonusStruct> bonusStructs = {};
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	float baseMaxLife = 100;
};

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UGameplaySave final : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FPlayerSave playerSave;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FBonusSave bonusSave;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FGameSave gameSave;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FMeteoSave meteoSave;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FMemberSave memberSave;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FTutoSave tutoSave;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FGameQuest quests;

public:
	UFUNCTION(BlueprintCallable)
	void SaveGameplay(const AMainCharacter* _player, const int dayToSave = -1);
	void SaveBonus(AMainCharacter* player);
	void SaveMeteo(AMainCharacter* _player);
	void SaveMembers(AMainCharacter* _player);
	void SaveQuests(const TArray<int>& _asset);
	UFUNCTION(BlueprintCallable)
	void SetIsTutorial(const bool isTutorial);
	void SaveTime();
	UFUNCTION(BlueprintCallable)
	void SaveTuto(bool _status);

	FORCEINLINE FPlayerSave GetPlayerSave() const { return playerSave; }
	FORCEINLINE FBonusSave GetBonusSave() const { return bonusSave; }
	FORCEINLINE FGameSave GetGameSave() const { return gameSave; }
	FORCEINLINE FGameQuest GetQuestSave() const { return quests; }
	FORCEINLINE FMemberSave GetMembersSave() const { return memberSave; }
	FORCEINLINE FTutoSave GetTutoSave() const { return tutoSave; }
};
