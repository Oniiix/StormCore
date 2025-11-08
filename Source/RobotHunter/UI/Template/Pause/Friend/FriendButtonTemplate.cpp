#include "FriendButtonTemplate.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "RobotHunter/UI/Menu/Pause/Friend/FriendSettings.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Styling/SlateTypes.h"

void UFriendButtonTemplate::SetStyle(const FButtonStyle& _style)
{
	btnFriend->SetStyle(_style);
}

void UFriendButtonTemplate::SetFriend(const FFriendsResult& friends_result)
{
	currentFriend = friends_result;
	const FOnlineUserPresence _presence = currentFriend.presence;

	SetStyle(_presence.bIsPlayingThisGame ? settings->GetInGameStyle() : settings->GetOnlineStyle());

	BIND_ACTION(btnFriend, OnClick, UFriendButtonTemplate, InviteFriend);

	SET_TEXT_STR(txtNameFriend->GetText(), FORMAT(formatPlayerName, currentFriend.name));
	SET_TEXT_STR(txtFriendOnline->GetText(),
	             FORMAT(formatOnlineStatus, _presence.bIsPlaying ? settings->InGameText() : settings->OnlineText()));
}

void UFriendButtonTemplate::SetSettings(UFriendSettings* _settings)
{
	settings = _settings;
}


void UFriendButtonTemplate::InviteFriend()
{
	if (UOnlineManagerSubsystem* oms = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
	{
		oms->SendInviteFriends(currentFriend.index);
	}
}

void UFriendButtonTemplate::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(btnFriend, OnHover, UFriendButtonTemplate, ChangeStyle);
}

void UFriendButtonTemplate::ChangeStyle()
{
	const FOnlineUserPresence _presence = currentFriend.presence;
	SetStyle(_presence.bIsPlayingThisGame ? settings->GetInGameStyle() : settings->GetOnlineStyle());
}
