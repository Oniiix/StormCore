#include "FriendRowTemplate.h"

#include "RobotHunter/UI/Menu/Pause/Friend/FriendSettings.h"
#include "RobotHunter/UI/Template/Pause/Friend/FriendButtonTemplate.h"
#include "Components/HorizontalBoxSlot.h"

void UFriendRowTemplate::SetSettings(UFriendSettings* _settings)
{
	settings = _settings;
}

bool UFriendRowTemplate::HasSpace() const
{
	return settings->GetNumberFriendPerRow() > row->GetChildrenCount();
}

void UFriendRowTemplate::AddFriend(const FFriendsResult& friends_result)
{
	if (UFriendButtonTemplate* _friendBtn = CreateWidget<UFriendButtonTemplate>(
		this, settings->GetFriendButtonTemplate()))
	{
		row->AddChild(_friendBtn);
		_friendBtn->SetSettings(settings);
		_friendBtn->SetFriend(friends_result);
		if (UHorizontalBoxSlot* _slot = Cast<UHorizontalBoxSlot>(_friendBtn->Slot))
		{
			//_slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			_slot->SetPadding(FMargin(15, 5));
		}
	}
}
