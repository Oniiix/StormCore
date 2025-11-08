// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkUI.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UNetworkUI::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UNetworkUI::Init()
{
	RefreshUI();
	BindButton();
}

void UNetworkUI::RefreshUI()
{
	const TObjectPtr<ACustomGameState> _gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());
	if (!_gameState)
	{
		return;
	}
	int _numPlayer = _gameState->GetPlayers().Num();
	for (size_t i = 0; i < _numPlayer; i++)
	{
		if (_gameState->GetPlayerOwner() == _gameState->GetPlayers()[i])
		{
			continue;
		}
		CreateSlotPlayerUI(_gameState->GetPlayers()[i]);
	}
}

void UNetworkUI::BindButton()
{
	BIND_ACTION_PARAM(btn_Return, OnClicked, UNetworkUI, OnClickedBtn_Return);
}

void UNetworkUI::CreateSlotPlayerUI(const TObjectPtr<AMainCharacter>& _player)
{
	const TObjectPtr<USlotPlayerUI> _slotPlayerUI = CreateWidget<USlotPlayerUI>(GetWorld(), slotPlayerUIWidget);
	playerListVerticalBox->AddChild(_slotPlayerUI);
	_slotPlayerUI->SetPlayer(_player);
	_slotPlayerUI->SetText(FText::FromString(_player->GetPlayerState()->GetPlayerName()));
}

void UNetworkUI::OnClickedBtn_Return()
{
	GetOwningPlayer()->bShowMouseCursor = false;
	SetFocus();
	this->RemoveFromParent();
}
