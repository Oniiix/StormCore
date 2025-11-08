// 


#include "CustomCheatManager.h"

#include "GameFramework/PlayerState.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/Pause/PauseMenu.h"
#include "RobotHunter/Utils/DebugUtils.h"


void UCustomCheatManager::SetUI(const FString& _menu, const FString& _subMenu) const
{
	AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	CHECK_NULL(_hud, "HUD is null");


	if (_menu.ToLower().Equals("pause"))
	{
		_hud->SetState(EStateUI::MainPause);
		const UPauseMenu* _pause = _hud->GetCurrentUI<UPauseMenu>();
		CHECK_NULL(_pause, "Pause menu is null");

		LOG_COLOR(true, "[CHEAT] Open pause menu", ORANGE);
		const TMap<FString, TFunction<void()>> functions =
		{
			{
				"game", (ShowGame(_pause))
			},
			{
				"friends", (ShowFriends(_pause))
			},
			{
				"options", (ShowOptions(_pause))
			},
			{
				"map", (ShowMap(_pause))
			},
		};

		const FString _lower = _subMenu.ToLower();
		if (functions.Contains(_lower))
		{
			_hud->SetState(EStateUI::MainPause);
			LOG_COLOR(true, "[CHEAT] Open " + _lower + " menu", ORANGE);
			functions[_lower]();
		}
		else
		{
			LOG_COLOR(true, "[CHEAT] Menu " + _lower + " doesn't exist", RED);
		}
	}
	else
	{
		const TMap<FString, TFunction<void()>> functions =
		{
			{"lobby", [&] { _hud->SetState(EStateUI::PlayerLobbyUI); }},
			{"game", [&] { _hud->SetState(EStateUI::PlayerGameUI); }},
			{"stock", [&] { _hud->SetState(EStateUI::HubStock); }},
			{"radio", [&] { _hud->SetState(EStateUI::HubWeather); }},
			{"start", [&] { _hud->SetState(EStateUI::HubStart); }},
		};

		if (functions.Contains(_menu))
		{
			_hud->SetState(EStateUI::MainPause);
			LOG_COLOR(true, "[CHEAT] Open " + _menu + " menu", ORANGE);
			functions[_menu]();
		}
		else
		{
			LOG_COLOR(true, "[CHEAT] Menu " + _menu + " doesn't exist", RED);
		}
	}
}

TFunction<void()> UCustomCheatManager::ShowOptions(const UPauseMenu* _pause) const
{
	return [&]
	{
		//_pause->CheatShowMenu(EStatusPause::Options);
	};
}

TFunction<void()> UCustomCheatManager::ShowGame(const UPauseMenu* _pause) const
{
	return [&]
	{
		//_pause->CheatShowMenu(EStatusPause::Game);
	};
}

TFunction<void()> UCustomCheatManager::ShowFriends(const UPauseMenu* _pause) const
{
	return [&]
	{
		//_pause->CheatShowMenu(EStatusPause::Friends);
	};
}

TFunction<void()> UCustomCheatManager::ShowMap(const UPauseMenu* _pause) const
{
	return [&]
	{
		//_pause->CheatShowMenu(EStatusPause::Map);
	};
}

void UCustomCheatManager::AddLife(const int _lifeToAdd)
{
	const AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetPawn());
	CHECK_NULL(_chara, "[CHEATMANAGER] Character is null");

	ULifeComponent* _life = _chara->GetLifeComponent();
	_life->RestoreLife(_lifeToAdd);

	LOG_COLOR(true, "[CHEATMANAGER] Life added to " + _chara->GetPlayerState()->GetPlayerName(), ORANGE);
}

void UCustomCheatManager::RemoveLife(const int _lifeToRemove)
{
	const AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetPawn());
	CHECK_NULL(_chara, "[CHEATMANAGER] Character is null");

	ULifeComponent* _life = _chara->GetLifeComponent();
	_life->Damage(_lifeToRemove, nullptr);

	LOG_COLOR(true, "[CHEATMANAGER] Life Removed to " + _chara->GetPlayerState()->GetPlayerName(), ORANGE);
}

void UCustomCheatManager::TakePlayerDamage(const int _idOfPlayer, const int _damage)
{
	const TObjectPtr<ACustomGameState> _state = Cast<ACustomGameState>(GetWorld()->GetGameState());
	CHECK_NULL(_state, "Custom game state is null");

	TArray<AMainCharacter*> Players = _state->GetPlayers();

	const AMainCharacter* _chara = Cast<AMainCharacter>(Players[_idOfPlayer]);
	CHECK_NULL(_chara, "[CHEATMANAGER] Character is null");

	AMainCharacter* _me = Cast<AMainCharacter>(GETFPC->GetPawn());

	ULifeComponent* _life = _chara->GetLifeComponent();
	_life->Damage(_damage, _me);

	LOG_COLOR(true, "[CHEATMANAGER] Make damage to player: " + _chara->GetPlayerState()->GetPlayerName(), ORANGE);
}
