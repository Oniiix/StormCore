// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/UI/Tuto/UI/DataAsset/TutorialHintAsset.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "ArenaManager.generated.h"

#define ARENA_MANAGER_USE_LOG false

class APrincipalGameMode;
class AArena;
class AMainCharacter;
class UAIsRefDataAsset;
class UAIsSettingsDataAsset;

/**
 * The manager of Arena
 */
UCLASS(Blueprintable, HideDropdown)
class ROBOTHUNTER_API UArenaManager : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaManagerReady);
	FOnArenaManagerReady onArenaManagerReady;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearAllGauntlet);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnClearAllGauntlet onClearAllGauntlet;

#pragma region Properties
	/// Arena
	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TObjectPtr<APrincipalGameMode> gameMode = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arena")
	bool loadAllArenas = false;
	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TArray<TObjectPtr<AArena>> listArenas;

	/// AI
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UAIsRefDataAsset> AIsRefs = nullptr;

	FTimerHandle timerDelayedStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arena", meta = (AllowPrivateAccess))
	int index = 0;



	UPROPERTY(EditAnywhere, Category = "ClearAll")
	TObjectPtr<UTutorialHintAsset> clearGauntletAsset = nullptr;

	int currentClearGauntletCount = 0;
	int gauntletCount = 0;

#pragma endregion

#pragma region Methods
public:
	/// Getter
	FORCEINLINE FOnArenaManagerReady& OnArenaManagerReady() { return onArenaManagerReady; }
	FORCEINLINE TObjectPtr<UAIsRefDataAsset> GetAIsRefs() { return AIsRefs; }

	/// <summary>
	/// Initialize the SpawnManager
	/// </summary>
	void Init(APrincipalGameMode* _gameMode);
	/// <summary>
	/// Add an arena to the listArenas
	/// </summary>
	/// <param name="_arena"> arena to add </param>
	void AddArena(AArena* _arena);

private:
	UFUNCTION()
	void AllPlayerJoined();
	/// <summary>
	/// Delay the start of evolution to collect all arenas in level
	/// </summary>
	void DelayedStart();

	UFUNCTION() void ClearArena(AAlienDoor* _exitGate, bool _giveBonus);

#pragma endregion
};
