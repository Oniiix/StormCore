#include "CustomGameMode.h"
#include "GameFramework/PlayerState.h"
#include "GPE/HovercraftPlayerStart.h"
#include "GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/SaveSystem/GameplaySave.h"
#include <Kismet/GameplayStatics.h>
#include "RobotHunter/Network/Tree/TreeManagerSubSystem.h"

ACustomGameMode::ACustomGameMode()
{
	bUseSeamlessTravel = true;

}

void ACustomGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	Init();
}

void ACustomGameMode::Init()
{
	LoadArenaLevel();
}

void ACustomGameMode::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToTransition, ActorList);

	ActorList.Add(sessionSetup);
}

void ACustomGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		UE_LOG(LogGameMode, Warning, TEXT("RestartPlayerAtPlayerStart: Player start not found"));
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnFor(NewPlayer, StartSpot);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}

	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);


		FinishRestartPlayer(NewPlayer, SpawnRotation);

		if (AHovercraftPlayerStart* _playerStart = Cast<AHovercraftPlayerStart>(StartSpot))
		{
			ACustomHandcarActor* _handcar = _playerStart->GetAttachParentActor() != nullptr ? Cast<ACustomHandcarActor>(_playerStart->GetAttachParentActor()) : nullptr;
			if (_handcar)
			{
				if (UCustomCharacterMovementComponent* _move = NewPlayer->GetPawn()->GetComponentByClass<UCustomCharacterMovementComponent>())
					_move->SetOnHandcar(true, _handcar);
			}

		}
	}
}






void ACustomGameMode::LoadArenaLevel()
{
	const FString& _name = GetWorld()->GetName();
	LOG_COLOR(CUSTOM_GAMEMODE_USE_LOG, "Current Level => " + _name, BLUE);
	if (customSubLevels.Contains(_name) && customSubLevels[_name].levelsPerDay.Num() > 0)
	{
		const int _num = customSubLevels[_name].levelsPerDay.Num();

		UGameplaySave* _save = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave();
		if (!_save) return;

		const int _realDay = _save->GetGameSave().day;

		if (customSubLevels[_name].levelsPerDay.Contains(_realDay))
		{
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, customSubLevels[_name].levelsPerDay[_realDay], true, false, FLatentActionInfo());
			LOG_COLOR(CUSTOM_GAMEMODE_USE_LOG, "LOAD => " + customSubLevels[_name].levelsPerDay[_realDay].GetAssetName(), FColor::Emerald);
		}
	}
}
