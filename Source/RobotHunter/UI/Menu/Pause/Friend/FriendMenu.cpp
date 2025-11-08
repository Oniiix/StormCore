#include "FriendMenu.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "FriendSettings.h"
#include "RobotHunter/UI/Template/Pause/Friend/FriendRowTemplate.h"

void UFriendMenu::RegisterEvents()
{
	Super::RegisterEvents();


	if (IS_LAN())
	{
		LOG_ERROR(FRIEND_MENU_USE_LOG, "Is in lan");
		return;
	}

	oms = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>();
	CHECK_NULL(oms, "OMS is null");

	BIND_ACTION(oms, OnFriendListUpdate, UFriendMenu, FriendsReceive);
}

void UFriendMenu::Refresh()
{
	Super::Refresh();
	SET_TEXT_STR(txtPlayerOnline->GetText(), INTSTR(0));
	SET_TEXT_STR(txtPlayerInGame->GetText(), INTSTR(0));

	if (!(IS_LAN()))
	{
		LoadFriends();
	}
}

void UFriendMenu::DisableAllWidgetInteractable()
{
	widgetToDisabled.Add(listFriend);
	Super::DisableAllWidgetInteractable();
}

void UFriendMenu::RefreshUI()
{
	listFriend->GetScrollbox()->ClearChildren();

	TArray<FFriendsResult> _friends = GetSortedFriends(ONLINE);
	const int _size = _friends.Num();


	int _online = 0;
	int _inGame = 0;

	for (int i = 0; i < _size; i++)
	{
		FFriendsResult _friend = _friends[i];
		const FOnlineUserPresence presence = _friend.presence;
		if (!presence.bIsOnline)
		{
			continue;
		}

		_online += presence.bIsOnline;
		_inGame += presence.bIsPlayingThisGame;

		UFriendRowTemplate* _hb = GetLastHorizontalBox();
		CHECK_NULL(_hb, "Friend row template is null");

		_hb->SetSettings(settings);
		if (_hb->HasSpace())
		{
			_hb->AddFriend(_friend);
		}

		SET_TEXT_STR(txtPlayerOnline->GetText(), INTSTR(_online));
		SET_TEXT_STR(txtPlayerInGame->GetText(), INTSTR(_inGame));
	}
}

void UFriendMenu::LoadFriends()
{
	CHECK_NULL(oms, "oms is null");

	oms->InitFriendList();
}


TArray<FFriendsResult> UFriendMenu::GetSortedFriends(ESortFriends _sort) const
{
	TArray<FFriendsResult> _result;
	TArray<FFriendsResult> _inGame;
	TArray<FFriendsResult> _online;
	TArray<FFriendsResult> _offline;

	for (FFriendsResult _friend : friends)
	{
		FOnlineUserPresence _presence = _friend.presence;

		if (_presence.bIsOnline)
		{
			if (_presence.bIsPlayingThisGame)
			{
				_inGame.Add(_friend);
			}
			else
			{
				_online.Add(_friend);
			}
		}
		else
		{
			_offline.Add(_friend);
		}
	}

	switch (_sort)
	{
	case ONLINE:
		_result.Append(_inGame);
		_result.Append(_online);
		_result.Append(_offline);
		break;

	case OFFLINE:
		_result.Append(_offline);
		_result.Append(_online);
		_result.Append(_inGame);
		break;
	}

	return _result;
}

void UFriendMenu::Init()
{
	Super::Init();
}

void UFriendMenu::FriendsReceive(const TArray<FFriendsResult>& _friends)
{
	friends = _friends;
	RefreshUI();
}

UFriendRowTemplate* UFriendMenu::CreateRow()
{
	const TSubclassOf<UFriendRowTemplate> _template = settings->GetFriendRowTemplate();
	CHECK_NULL_WITH_RETURN(_template, "Template of friend row is null", return nullptr);

	UFriendRowTemplate* _row = CreateWidget<UFriendRowTemplate>(this, _template);
	_row->SetSettings(settings);
	listFriend->GetScrollbox()->AddChild(_row);
	return _row;
}

UFriendRowTemplate* UFriendMenu::GetLastHorizontalBox()
{
	const TObjectPtr<UScrollBox> scrollbox = listFriend->GetScrollbox();
	if (scrollbox->GetChildrenCount() == 0) //Check HB
	{
		const UFriendRowTemplate* value = CreateRow(); //Non ?

		CHECK_NULL_WITH_RETURN(value, "Cannot create row", return nullptr);

		return GetLastHorizontalBox(); //Back to getlastHB
	}

	UFriendRowTemplate* _last = Cast<UFriendRowTemplate>(
		scrollbox->GetAllChildren()[scrollbox->GetChildrenCount() - 1]); //Oui -> get last hb in child

	if (!_last->HasSpace()) //check size
	{
		const UFriendRowTemplate* value = CreateRow(); //Non
		CHECK_NULL_WITH_RETURN(value, "Cannot create row", return nullptr);

		return GetLastHorizontalBox();
	}
	return _last;
}
