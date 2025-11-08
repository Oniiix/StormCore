#include "PauseMenu.h"

#include "Components/HorizontalBox.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/Options/OptionMenu.h"
#include "RobotHunter/Utils/DebugUtils.h"

class AMainCharacter;

void UPauseMenu::DisableAllWidgetInteractable()
{
	auto _buttons = {Pause_BtnGame, btnFriends, Pause_btnMap};
	for (const TObjectPtr<UButtonPrefab>& ButtonPrefab : _buttons)
	{
		widgetToDisabled.Add(ButtonPrefab);
	}

	for (UWidget* AllChild : menuSwitcher->GetAllChildren())
	{
		if (UPauseFrame* _frame = Cast<UPauseFrame>(AllChild))
		{
			_frame->DisableAllWidgetInteractable();
		}
	}

	Super::DisableAllWidgetInteractable();
}

void UPauseMenu::Init()
{
	Super::Init();

	GameInstance = GetWorld()->GetGameInstance<UCustomGameInstance>();
	const bool bIsOnlineMode = GameInstance->GetOnlineMode();
	const EGameStatus GameStatus = GameInstance->GetGameStatus();

	const ESlateVisibility FriendsVisibility = bIsOnlineMode ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	btnFriends->SetVisibility(FriendsVisibility);

	if (FriendsVisibility == ESlateVisibility::Visible)
	{
		const bool bShouldEnableFriends = GameStatus == HUB;
		btnFriends->SetIsEnabled(bShouldEnableFriends);
	}
	else
	{
		btnFriends->SetIsEnabled(false);
		btnFriends->GetParent()->RemoveFromParent();
	}


	for (const TObjectPtr<UWidget>& Menu : menuSwitcher->GetAllChildren())
	{
		if (UPauseFrame* PauseFrame = Cast<UPauseFrame>(Menu))
		{
			PauseFrame->Setup(hud);
			PauseFrame->SetOwner(this);
		}
	}
}

void UPauseMenu::ClearSelected()
{
	if (!horizontalMenu)
	{
		return;
	}
	const TArray<UWidget*> Children = horizontalMenu->GetAllChildren();
	for (UWidget* Child : Children)
	{
		if (!Child)
		{
			continue;
		}
		if (UButtonPrefab* Button = Cast<UButtonPrefab>(Child))
		{
			if (Button->CanKeepSelected() && Button->IsSelectedButton())
			{
				Button->Callback_OnUnSelected();
			}
			continue;
		}

		if (UPanelWidget* Panel = Cast<UPanelWidget>(Child))
		{
			const TArray<UWidget*> PanelChildren = Panel->GetAllChildren();
			for (UWidget* PanelChild : PanelChildren)
			{
				if (!PanelChild)
				{
					continue;
				}

				if (UButtonPrefab* PanelButton = Cast<UButtonPrefab>(PanelChild))
				{
					if (PanelButton->CanKeepSelected() && PanelButton->IsSelectedButton())
					{
						PanelButton->Callback_OnUnSelected();
					}
				}
			}
		}
	}
}

void UPauseMenu::RegisterEvents()
{
	Super::RegisterEvents();

	/*auto _buttons = {btnGame, btnFriends, btnMap};
	for (const TObjectPtr<UButtonPrefab>& ButtonPrefab : _buttons)
	{
		BIND_ACTION(ButtonPrefab, OnSelected, UPauseMenu, ClearSelected);
	}*/

	AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter());
	CHECK_NULL(_chara, "Character is null");

	BIND_ACTION(Pause_btnMap, OnClick, UPauseMenu, ShowMap);

	BIND_ACTION(_chara, OnQuitMenu, UPauseMenu, QuitMenu);
}

void UPauseMenu::QuitMenu()
{
	onQuit.Broadcast();
	onHideMap.Broadcast();

	GetGameInstance<UCustomGameInstance>()->SetInPause(false);
}

void UPauseMenu::Refresh()
{
	Super::Refresh();
	onShow.Broadcast();

	Pause_BtnGame->Callback_OnSelected();

	GetGameInstance<UCustomGameInstance>()->SetInPause(true);
}

void UPauseMenu::ShowFriends()
{
	RefreshUI(static_cast<int>(EStatusPause::Friends));
}

void UPauseMenu::ShowOption()
{
	RefreshUI(static_cast<int>(EStatusPause::Options));
}

void UPauseMenu::ShowGame()
{
	RefreshUI(static_cast<int>(EStatusPause::Game));
}


void UPauseMenu::ShowMap()
{
	RefreshUI(static_cast<int>(EStatusPause::Map));
	backgroundMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UPauseMenu::RefreshUI(const int index) const
{
	EStatusPause _state = static_cast<EStatusPause>(index);
	if (_state == EStatusPause::Map)
	{
		hud->SetState(EStateUI::Map3D);
		onShowMap.Broadcast();
		return;
	}

	onHideMap.Broadcast();

	if (menuSwitcher->GetChildrenCount() < index + 1)
		return;

	backgroundMenu->SetVisibility(ESlateVisibility::Visible);

	menuSwitcher->SetActiveWidgetIndex(index);
	Cast<UBaseMenu>(menuSwitcher->GetWidgetAtIndex(index))->Refresh();

	onRefreshPause.Broadcast(_state);
}

UOptionFrame* UPauseMenu::GetOption() const
{
	return Cast<UOptionFrame>(menuSwitcher->GetWidgetAtIndex(static_cast<int>(EStatusPause::Options)));
}
