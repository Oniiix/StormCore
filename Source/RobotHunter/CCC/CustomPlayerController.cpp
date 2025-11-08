// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "OnlineSubsystemUtils.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "OnlineStats.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "RobotHunter/Game/CustomGameState.h"


ACustomPlayerController::ACustomPlayerController()
{
	networkAI = CreateDefaultSubobject<UNetworkAIComponent>("NetworkAI");
	AddOwnedComponent(networkAI);
	networkGPE = CreateDefaultSubobject<UNetworkGPEComponent>("NetWorkGPE");
	AddOwnedComponent(networkGPE);
}

#pragma region Members
void ACustomPlayerController::SetRightArm(TSubclassOf<AMembersActor> _rightArm)
{
	LOG_COLOR(CUSTOM_PLAYER_CONTROLLER_USE_LOG, "Controller => SetRightArm", ORANGE);
	rightArm = _rightArm;
	if (!HasAuthority())
		ServerRpc_SetRightArm(_rightArm);
	else
		mainCharacter->SetRightArm(rightArm);
}

void ACustomPlayerController::ServerRpc_SetRightArm_Implementation(TSubclassOf<AMembersActor> _rightArm)
{
	LOG_COLOR(CUSTOM_PLAYER_CONTROLLER_USE_LOG, "Controller RPC => SetRightArm", YELLOW);
	rightArm = _rightArm;
	mainCharacter->SetRightArm(rightArm);
}

void ACustomPlayerController::SetLeftArm(TSubclassOf<AMembersActor> _leftArm)
{
	leftArm = _leftArm;
	if (!HasAuthority())
		ServerRpc_SetLeftArm(_leftArm);
	else
		mainCharacter->SetLeftArm(leftArm);
}

void ACustomPlayerController::ServerRpc_SetLeftArm_Implementation(TSubclassOf<AMembersActor> _leftArm)
{
	leftArm = _leftArm;
	mainCharacter->SetLeftArm(leftArm);
}

void ACustomPlayerController::SetCalf(TSubclassOf<AMembersActor> _calf)
{
	calf = _calf;
	if (!HasAuthority())
		ServerRpc_SetCalf(_calf);
	else
		mainCharacter->SetCalf(calf);
}

void ACustomPlayerController::ServerRpc_SetCalf_Implementation(TSubclassOf<AMembersActor> _calf)
{
	calf = _calf;
	mainCharacter->SetCalf(calf);
}

void ACustomPlayerController::SetThigh(TSubclassOf<AMembersActor> _thigh)
{
	thigh = _thigh;
	if (!HasAuthority())
		ServerRpc_SetThigh(_thigh);
	else
		mainCharacter->SetThigh(thigh);
}
void ACustomPlayerController::ServerRpc_SetThigh_Implementation(TSubclassOf<AMembersActor> _thigh)
{
	thigh = _thigh;
	mainCharacter->SetThigh(thigh);
}
#pragma endregion



void ACustomPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (GetPawn())
	{
		onPawnRegistered.Broadcast(mainCharacter = Cast<AMainCharacter>(GetPawn()));
		if (mainCharacter && HasAuthority())
			mainCharacter->SetupMembers(this);
	}
}


void ACustomPlayerController::NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest)
{
	Super::NotifyLoadedWorld(WorldPackageName, bFinalDest);
	currentMembers.Empty();
	currentMembers.Add(rightArm);
	currentMembers.Add(leftArm);
	currentMembers.Add(calf);
	currentMembers.Add(thigh);
	//LOG_COLOR(true, "Notify", YELLOW);
	//hud->Regenerate();
	//hud->SetState(EStateUI::Loading);
	//if (bFinalDest)
	//{
	//	//LOG_COLOR(true, "Load => 50%", ORANGE);
	//
	//	TArray<AActor*> _actors;
	//	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), ULoadingInterface::StaticClass(), _actors);
	//	loadingActorCount = _actors.Num();
	//	for (int i = 0; i < loadingActorCount; i++)
	//		Cast<ILoadingInterface>(_actors[i])->OnFinishLoad().AddUObject(this, &ACustomPlayerController::ActorFinishLoading);
	//	//LOG_COLOR(true, "Number of Interafce => " + FString::FromInt(loadingActorCount), MAGENTA);
	//}
	//else
	//{
	//	//LOG_COLOR(true, "Load => 25%", ORANGE); 
	//}
	//
	////LOG_COLOR(true, "Load Wordl => " + WorldPackageName.ToString(), ORANGE);
}


void ACustomPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
	//TODO FIX le crash  plus tard
 	//ACustomGameState* _gameState = GetWorld()->GetGameState<ACustomGameState>();
	/*if (_gameState && !_gameState->IsInGame())
		hud->SetState(EStateUI::Loading);*/
}


void ACustomPlayerController::UpdateLeaderboard(const FName& _statName, int32 _score)
{
    LOG_COLOR(false, "Update Leaderboard Started", ORANGE);

    const IOnlineSubsystem* _onlineSub = Online::GetSubsystem(GetWorld());
    if (_onlineSub)
    {
        LOG_COLOR(false, "Found Online Subsystem", ORANGE);

        IOnlineLeaderboardsPtr _leaderboards = _onlineSub->GetLeaderboardsInterface();
        if (_leaderboards.IsValid())
        {
            LOG_COLOR(false, "Found Leaderboards Interface", ORANGE);

            // Obtenez un identifiant unique pour le joueur
            const FUniqueNetIdRepl _playerId = PlayerState->GetUniqueId();
            if (_playerId.IsValid())
            {
                LOG_COLOR(false, "Valid Player ID Found. Preparing Stats Update", ORANGE);

                // Préparer les données des scores
                FOnlineLeaderboardWrite _writeLeaderboard;
                _writeLeaderboard.LeaderboardNames.Add(_statName); // Nom défini sur la plateforme
                _writeLeaderboard.RatedStat = _statName;
                _writeLeaderboard.DisplayFormat = ELeaderboardFormat::Number;
                _writeLeaderboard.SortMethod = ELeaderboardSort::Descending;
                _writeLeaderboard.UpdateMethod = ELeaderboardUpdateMethod::KeepBest;
            	
                // Attribuer la valeur au score
                _writeLeaderboard.SetIntStat(_statName, _score);

                // Écrire les scores au leaderboard
                _leaderboards->WriteLeaderboards(
                    FName(TEXT("Test_Achievement")), // Nom du leaderboard
                    *_playerId,
                    _writeLeaderboard
                );

                LOG_COLOR(false, "Leaderboard update requested.", ORANGE);
            }
            else
            {
                LOG_COLOR(false, "Player ID is invalid. Leaderboard update aborted.", RED);
            }
        }
        else
        {
            LOG_COLOR(false, "Leaderboard Interface Invalid", RED);
        }
    }
    else
    {
        LOG_COLOR(false, "Online Subsystem Not Found", RED);
    }
}




void ACustomPlayerController::BeginPlay()
{
	currentMembers.Empty();
	currentMembers.Add(rightArm);
	currentMembers.Add(leftArm);
	currentMembers.Add(calf);
	currentMembers.Add(thigh);

	Super::BeginPlay();
	hud = Cast<AMainHUD>(MyHUD);
}


void ACustomPlayerController::ActorFinishLoading()
{
	currentActorLoad++;
	if (currentActorLoad == loadingActorCount)
	{
		//LOG_COLOR(true, "toutr est load chef!", FColor::Emerald);
		ACustomGameState* _gameState = GetWorld()->GetGameState<ACustomGameState>();
		onLoadFinish.Broadcast(_gameState->IsInGame() ? EStateUI::PlayerGameUI : EStateUI::PlayerLobbyUI);
	}
}


void ACustomPlayerController::CustomClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	if (MyHUD != NULL)
	{
		//LOG_COLOR(true, "Test", BLUE);
		//hud->SetState(EStateUI::Loading);
		//hud->Regenerate();
		return;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save HUDs into a map

	MyHUD = GetWorld()->SpawnActor<AHUD>(NewHUDClass, SpawnInfo);
}

void ACustomPlayerController::KickCustom_Implementation()
{
	if (UOnlineManagerSubsystem* _onlineManager = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
		_onlineManager->QuitServer();
}