#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/Menu/Pause/PauseFrame.h"
#include "RobotHunter/UI/Template/Prefab/ScrollBoxPrefab/ScrollboxPrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "FriendMenu.generated.h"

#define FRIEND_MENU_USE_LOG false

class UFriendRowTemplate;
class UFriendSettings;

UENUM()
enum ESortFriends
{
	ONLINE,
	OFFLINE
};


UCLASS(HideDropdown)
class ROBOTHUNTER_API UFriendMenu final : public UPauseFrame
{
	GENERATED_BODY()

	/** The settings of friend menu */
	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	TObjectPtr<UFriendSettings> settings;


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> txtPlayerOnline;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> txtPlayerInGame;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollboxPrefab> listFriend;

	UPROPERTY()
	UOnlineManagerSubsystem* oms;
	UPROPERTY()
	TArray<FFriendsResult> friends;


	/** Method called when the OMS response with request friend */
	UFUNCTION()
	void FriendsReceive(const TArray<FFriendsResult>& _friends);
	/**
	 * Load all friend from steam
	 */
	void LoadFriends();
	/**
	 * Refresh the list with friends
	 */
	void RefreshUI();
	/**
	 * Create row to add friend
	 * @return UFriendRowTemplate*
	 */
	UFriendRowTemplate* CreateRow();

	/**
	 * Get the last row, or if not created or filled, create new row
	 * @return UFriendRowTemplate*
	 */
	UFriendRowTemplate* GetLastHorizontalBox();
	/**
	 * Get sorted list by parameter
	 * @param _sort ESortFriend
	 * @return TArray<FFriendsResult>
	 */
	TArray<FFriendsResult> GetSortedFriends(ESortFriends _sort) const;


	virtual void Init() override;
	virtual void RegisterEvents() override;
	virtual void Refresh() override;
	virtual void DisableAllWidgetInteractable() override;
};
