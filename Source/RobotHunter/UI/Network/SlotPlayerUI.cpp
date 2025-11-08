// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotPlayerUI.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameState.h"


void USlotPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void USlotPlayerUI::Init()
{
	BIND_ACTION_PARAM(btnKick_1, OnClicked, USlotPlayerUI, KickButtonClicked);
	BIND_ACTION_PARAM(btnBan, OnClicked, USlotPlayerUI, BanButtonClicked);
}

void USlotPlayerUI::SetText(const FText& _text)
{
	SET_TEXT(txtName_1, _text);
}

void USlotPlayerUI::KickButtonClicked()
{
	const TObjectPtr<ACustomGameMode> _customGameMode = Cast<ACustomGameMode>(GetWorld()->GetAuthGameMode());
	if (!_customGameMode)
	{
		return;
	}
	const TObjectPtr<ACustomGameSession> _customGameSession = Cast<ACustomGameSession>(
		_customGameMode->GetGameSession());
	if (!_customGameSession)
	{
		return;
	}
	const TObjectPtr<APlayerController> _playerController = *_customGameSession->GetPlayerList().Find(
		player->GetPlayerState()->GetUniqueId());
	if (!_playerController)
	{
		return;
	}
	_customGameSession->KickPlayerCustom(_playerController);
	player = nullptr;
	this->RemoveFromParent();
}

void USlotPlayerUI::BanButtonClicked()
{
	const TObjectPtr<ACustomGameMode> _customGameMode = Cast<ACustomGameMode>(GetWorld()->GetAuthGameMode());
	if (!_customGameMode)
	{
		return;
	}
	const TObjectPtr<ACustomGameSession> _customGameSession = Cast<ACustomGameSession>(
		_customGameMode->GetGameSession());
	if (!_customGameSession)
	{
		return;
	}
	_customGameSession->BanPlayerCustom(player->GetPlayerState()->GetUniqueId());
	player = nullptr;
	this->RemoveFromParent();
}
