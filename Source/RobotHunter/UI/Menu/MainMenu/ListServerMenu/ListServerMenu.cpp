#include "ListServerMenu.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/MainMenu/ListServerMenu/ListServerSettings.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UListServerMenu::RegisterEvents()
{
	oms = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>();

	BIND_ACTION(backButton, OnClick, UListServerMenu, Back);
	BIND_ACTION(refreshButton, OnClick, UListServerMenu, Refresh);
	BIND_ACTION_PARAM(checkFriendsOnly, onCheckChange, UListServerMenu, RefreshServer);

	if (!oms)
	{
		return;
	}

	Super::RegisterEvents();
	BIND_ACTION(oms, OnServerFound, UListServerMenu, ServerFound);

	BIND_ACTION(inputServer, OnTextChange, UListServerMenu, FindServer);
}


void UListServerMenu::Back()
{
	hud->SetState(EStateUI::MainMain);
	oms->CancelFindServer();
}

void UListServerMenu::Refresh()
{
	scrollServersList->GetScrollbox()->ClearChildren();
	RefreshServer(checkFriendsOnly->IsChecked());
}

void UListServerMenu::Init()
{
	Super::Init();
	settings->Setup(hud);
}

void UListServerMenu::ServerFound(const TArray<FServerResult>& _servers)
{
	servers = _servers;
	LoadListServers(_servers);
}

void UListServerMenu::LoadListServers(const TArray<FServerResult>& _servers) const
{
	scrollServersList->GetScrollbox()->ClearChildren();
	const FText textFormat = TEXTSTR(FORMAT(settings->GetTextServerFound().ToString(), INTSTR(servers.Num())));
	SET_TEXT(labelNumServer->GetText(), textFormat);

	for (FServerResult _result : _servers)
	{
		CreateWidgetServer(_result);
	}

	circleLoad->SetVisibility(ESlateVisibility::Hidden);
	refreshButton->SetIsEnabled(true);
	inputServer->SetIsEnabled(true);
}

void UListServerMenu::CreateWidgetServer(const FServerResult& _result) const
{
	USlotServerTemplate* _widget = CreateWidget<USlotServerTemplate>(GetWorld(), settings->GetSlotServerTemplate());
	scrollServersList->GetScrollbox()->AddChild(_widget);
	_widget->InitServer(this, _result);
}

void UListServerMenu::FindServer(const FString& _newInput)
{
	circleLoad->SetVisibility(ESlateVisibility::Visible);
	refreshButton->SetIsEnabled(false);

	if (_newInput.IsEmpty())
	{
		LoadListServers(servers);
		return;
	}

	TArray<FServerResult> _serversFound;
	const size_t _size = servers.Num();
	for (int i = 0; i < _size; i++)
	{
		if (servers[i].sessionName.Contains(_newInput))
		{
			_serversFound.Add(servers[i]);
		}
	}
	LoadListServers(_serversFound);
}

void UListServerMenu::RefreshServer(const bool _friendOnly)
{
	circleLoad->SetVisibility(ESlateVisibility::Visible);
	SET_TEXT(labelNumServer->GetText(), settings->GetTextServerSearch());

	_friendOnly ? oms->FindFriendServer() : oms->FindServer();

	refreshButton->SetIsEnabled(false);
	inputServer->SetIsEnabled(false);
}
