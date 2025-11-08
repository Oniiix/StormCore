// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaManager.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/AI/AIsRefDataAsset.h"
#include "RobotHunter/AI/Settings/AIsSettingsDataAsset.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include <RobotHunter/Game/CustomGameState.h>
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"

void UArenaManager::Init(APrincipalGameMode* _gameMode)
{
	gameMode = _gameMode;

	/// Check gameMode availability
	if (!gameMode)
	{
		LOG_ERROR(ARENA_MANAGER_USE_LOG, "GameMode is null !!");
		return;
	}

	/// Check AIsRefs availability
	if (!AIsRefs)
	{
		LOG_ERROR(ARENA_USE_LOG, "The AIsRefs is null !!");
		return;
	}
	if (!AIsRefs->CheckAIsRef())
	{
		LOG_ERROR(ARENA_USE_LOG, "The references of AIsRefs are incomplete !!");
		return;
	}

	/// Wait every player joined
	UPlayerManager* _playerManager = gameMode->GetPlayerManager();
	if (_playerManager)
		_playerManager->OnAllPlayerJoin().AddUniqueDynamic(this, &UArenaManager::AllPlayerJoined);
	else
		LOG_ERROR(ARENA_MANAGER_USE_LOG, "Failed to get PlayerManager in ArenaManager");
}

void UArenaManager::AllPlayerJoined()
{
	// Delay anyway for arena registration
	LOG_COLOR(true, "ALL PLAYER JOINED", PINK);
	GetWorld()->GetTimerManager().SetTimer(timerDelayedStart, this, &UArenaManager::DelayedStart, 0.1f, false);
}

void UArenaManager::DelayedStart()
{
	GetWorld()->GetTimerManager().ClearTimer(timerDelayedStart);

	/// Check listArenas availability
	if (listArenas.IsEmpty())
	{
		LOG_ERROR(ARENA_MANAGER_USE_LOG, "ListArenas of ArenaManager is empty => No Evolution !!");
		return;
	}

	/// Load all arenas forever
	if (loadAllArenas)
	{
		const int _arenaCount = listArenas.Num();
		for (int i = 0; i < _arenaCount; i++)
		{
			listArenas[i]->SetAlwaysLoaded(true);
			listArenas[i]->Load(true);
		}
	}

	onArenaManagerReady.Broadcast();
}

void UArenaManager::ClearArena(AAlienDoor* _exitGate, bool _giveBonus)
{
	currentClearGauntletCount++;
	if (gauntletCount == currentClearGauntletCount)
		onClearAllGauntlet.Broadcast();
}


void UArenaManager::AddArena(AArena* _arena)
{
	LOG_COLOR(ARENA_MANAGER_USE_LOG, "Add " + _arena->ACTOR_NAME + " to arenaManager", GREEN);
	listArenas.Add(_arena);
	if (_arena->GetFinalArena())
	{
		gauntletCount++;
		_arena->OnClearArena().AddUniqueDynamic(this, &UArenaManager::ClearArena);
	}
}
