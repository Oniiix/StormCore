#include "PrincipalGameMode.h"
#include "RobotHunter/AI/Arena/ArenaManager.h"
#include "RobotHunter/Game/Managers/Handcar/StopHandcarManager.h"
#include <RobotHunter/Game/Managers/LightningGenerator/LightningGenerator.h>
#include <RobotHunter/Utils/DebugUtils.h>
#include "RobotHunter/Game/Managers/Handcar/StopHandcarManager.h"
#include "../CustomGameState.h"
#if WITH_EDITORONLY_DATA
#include "CustomEditor/Public/CustomEditorSettings.h"
#endif
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/SaveSystem/GameplaySave.h"
#include <Kismet/GameplayStatics.h>
#include "RobotHunter/Network/Tree/TreeManagerSubSystem.h"
#include "../CustomGameState.h"

#define INIT_DELAY 5.f
#define NUMBER_OF_PLAYER 2

void APrincipalGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (AActor* _sessionSetup = UGameplayStatics::GetActorOfClass(GetWorld(), ASessionSetup::StaticClass()))
	{
		LOG_COLOR(PRINCIPAL_GAMEMODE_USE_LOG, "Get seamles SessionInfo", YELLOW);
		sessionSetup = Cast<ASessionSetup>(_sessionSetup); // reestablish the linkage

		if (ACustomGameSession* _gs = Cast<ACustomGameSession>(GameSession))
			_gs->SetSessionSetup(sessionSetup); // cache it in GameState so it's easily accessible client-side
	}
}

void APrincipalGameMode::Init()
{
		
#if WITH_EDITORONLY_DATA // Development
	// Use seamless tool 
	bUseSeamlessTravel = GetMutableDefault<UCustomEditorSettings>()->GetSeamless();
	// Get number of client wanted in multiplayer options
	const ULevelEditorPlaySettings* _playInSettings = GetDefault<ULevelEditorPlaySettings>();
	_playInSettings->GetPlayNumberOfClients(numberOfPlayer);
#else // Ship
	bUseSeamlessTravel = true;
	numberOfPlayer = NUMBER_OF_PLAYER;
#endif

	playerManager = NewObject<UPlayerManager>(this);
	if (!playerManager)
	{
		LOG_ERROR(PRINCIPAL_GAMEMODE_USE_LOG, "Failed to create playerManager in PrincipaleGameMode");
		return;
	}

	/// Create arenaManager
	if (!arenaManagerRef)
	{
		LOG_ERROR(PRINCIPAL_GAMEMODE_USE_LOG, "arenaManagerRef is null in PrincipaleGameMode");
		return;
	}
	arenaManager = NewObject<UArenaManager>(this, arenaManagerRef, FName("SpawnManager"));
	if (!arenaManager)
	{
		LOG_ERROR(PRINCIPAL_GAMEMODE_USE_LOG, "Failed to create arenaManager in PrincipaleGameMode");
		return;
	}

	/// Handle seamless or not travel
	if (bUseSeamlessTravel)
	{
		LOG_COLOR(PRINCIPAL_GAMEMODE_USE_LOG, "USE SEAMLESS", PINK);
		arenaManager->Init(this);
		playerManager->Init();
	}
	else
		GetWorld()->GetTimerManager().SetTimer(timerDelayedInit, this, &APrincipalGameMode::DelayedInit, 1.f);

	Super::Init();
}


void APrincipalGameMode::DelayedInit()
{
	if (GetWorld()->GetNumPlayerControllers() == numberOfPlayer && !bUseSeamlessTravel)
	{
		LOG_COLOR(PRINCIPAL_GAMEMODE_USE_LOG, "NOT SEAMLESS", YELLOW);
		GetWorld()->GetTimerManager().ClearTimer(timerDelayedInit);
		arenaManager->Init(this);
		playerManager->Init();
	}
}

void APrincipalGameMode::InitGameState()
{
	Super::InitGameState();
	UCustomGameInstance* _gi = GetGameInstance<UCustomGameInstance>();
	ACustomGameState* _gs = Cast<ACustomGameState>(GameState);
	if (_gi && _gs)
		_gs->SetCurrentMeteo(GetWorld()->SpawnActor<AWeather>(_gi->GetCurrentWeather()));
}
