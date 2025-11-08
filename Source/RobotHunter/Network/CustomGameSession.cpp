// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameSession.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "GameFramework/PlayerState.h"


void ACustomGameSession::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//RegisterPlayer(NewPlayer, NewPlayer->PlayerState->GetUniqueId(), false);
	//RegisterPlayerController(NewPlayer, NewPlayer->PlayerState->GetUniqueId());
	//LOG_COLOR_TIME(GAME_SESSION_USE_LOG, "PostLogin Player => " + NewPlayer->GetName(), GREEN, 30);

}


void ACustomGameSession::BanPlayerCustom(const FUniqueNetIdRepl& UniqueId)
{
	//LOG_COLOR_TIME(GAME_SESSION_USE_LOG, "BanPlayerCustom add in list", RED, 30);
	//sessionSetup->GetSessionInfo().banList.Add(UniqueId);
	//
	//UOnlineManagerSubsystem* oms = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>();
	//oms->AddBanList(UniqueId->ToString() + "/");
	//LOG_COLOR_TIME(GAME_SESSION_USE_LOG, "GET BAN LIST => " + oms->GetBanList(), GREEN, 30);
	//oms->UpdateServer(BAN_LIST,oms->GetBanList());
	//KickPlayerCustom(*playerList.Find(UniqueId));
}

void ACustomGameSession::KickPlayerCustom(APlayerController* _playerController)
{
	//ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(_playerController);
	//_customPlayerController->KickCustom();
}

void ACustomGameSession::NotifyLogout(const APlayerController* PC)
{
	Super::NotifyLogout(PC);
	//UnregisterPlayer(PC);
	//UnRegisterPlayerController(PC->PlayerState->GetUniqueId());
	//LOG_COLOR(GAME_SESSION_USE_LOG, "Player [" + PC->PlayerState->GetPlayerName() + "] => LogOut", RED);
}

void ACustomGameSession::RegisterPlayerController(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId)
{
	//if (sessionSetup)
	//	sessionSetup->GetSessionInfo().playerList.Add(UniqueId, NewPlayer);
}

void ACustomGameSession::UnRegisterPlayerController(const FUniqueNetIdRepl& UniqueId)
{
	//if (sessionSetup)
	//	sessionSetup->GetSessionInfo().playerList.Remove(UniqueId);
}