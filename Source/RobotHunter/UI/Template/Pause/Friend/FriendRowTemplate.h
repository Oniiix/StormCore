#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/UI.h"
#include "FriendRowTemplate.generated.h"


#define FRIEND_ROW_TEMPLATE_USE_LOG true


class UFriendSettings;

UCLASS(HideDropdown)
class ROBOTHUNTER_API UFriendRowTemplate final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> row;

	UPROPERTY()
	TObjectPtr<UFriendSettings> settings;

public:
	void SetSettings(UFriendSettings* _settings);
	bool HasSpace() const;
	void AddFriend(const FFriendsResult& friends_result);
};
