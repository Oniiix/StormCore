#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Menu/Pause/Friend/FriendSettings.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "FriendButtonTemplate.generated.h"

class UFriendSettings;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UFriendButtonTemplate final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> btnFriend;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> txtNameFriend;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> txtFriendOnline;

	UPROPERTY()
	TObjectPtr<UFriendSettings> settings;

	UPROPERTY()
	FFriendsResult currentFriend;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatPlayerName = "<normal>{0}</>";


	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatOnlineStatus = "<normal>{0}</>";

public:
	void SetFriend(const FFriendsResult& friends_result);
	void SetSettings(UFriendSettings* _settings);

protected:
	virtual void RegisterEvents() override;

private:
	UFUNCTION()
	void InviteFriend();

	UFUNCTION()
	void ChangeStyle();
	void SetStyle(const FButtonStyle& _style);
};
