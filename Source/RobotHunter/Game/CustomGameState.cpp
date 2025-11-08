// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"
#include "../CCC/Character/MainCharacter.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"

ACustomGameState::ACustomGameState()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void ACustomGameState::SetCurrentMeteo(AWeather* _weather)
{
	currentMeteo = _weather->GetMeteoType();
	currentWeather = _weather;
	LOG_COLOR(true, "Set Current Meteo in GameState", BLUE);
	onMeteoInit.Broadcast(); 
}

//TODO Remove si inutile
bool ACustomGameState::Register(AMainCharacter* _player, const bool _isOwner)
{		
	if (players.Contains(_player)) return false;
	if (_isOwner)
		playerOwner = _player;

	players.Add(_player);
	onPlayerJoin.Broadcast(_player);
	//_player->PlayerInitVoiceChat(); //TODO Fix l'erreur  thib stp (c'est pas moi)

	if (players.Num() == UOnlineManagerSubsystem::GetPlayerMaxCount())
		onMaxPlayerCountReached.Broadcast();
	return true;
}

bool ACustomGameState::Unregister(AMainCharacter* _player)
{
	if (!players.Contains(_player)) return false;
	players.Remove(_player);
	onPlayerQuit.Broadcast();
	return true;
}
//


void ACustomGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	isInGame = startStatus != EGameStatus::HUB && startStatus != EGameStatus::MAINMENU;

	UCustomGameInstance* _cgi = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	CHECK_NULL(_cgi, "Custom game instance is null");
	_cgi->SetGameStatus(startStatus);
	stopHandcarManager = NewObject<UStopHandcarManager>(this);
}




#pragma region Online
void ACustomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomGameState, currentWeatherManager);
}

void ACustomGameState::OnRep_UpdateWeatherManager()
{
	if (currentWeatherManager)
	{
		currentWeatherManager->SelectWeather();
	}
}
#pragma endregion