// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../BaseSettings.h"
#include "CoreMinimal.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "FriendSettings.generated.h"

class UFriendRowTemplate;
class UFriendButtonTemplate;

USTRUCT()
struct FStyleSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FSlateBrush active;
	UPROPERTY(EditAnywhere)
	FSlateBrush hover;
};


/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UFriendSettings final : public UBaseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Template")
	TSubclassOf<UFriendButtonTemplate> friendButtonReference;
	UPROPERTY(EditAnywhere, Category = "Template")
	TSubclassOf<UFriendRowTemplate> friendRowReference;


	/**
	 * The brush set if the friend is in game.
	 * He was set in normal style
	 */
	UPROPERTY(EditAnywhere, Category = "Texture")
	FButtonStyle brushFriendInGame;

	/**
	* The brush set if the friend is disconnected.
	 * He was set in normal style
	 */
	UPROPERTY(EditAnywhere, Category = "Texture")
	FButtonStyle brushFriendOnline;


	/* The number of friends per line */
	UPROPERTY(EditAnywhere, Category = "Friend")
	int numberFriendPerRow = 2;

	/* Text displayed when player is in game */
	UPROPERTY(EditAnywhere, Category = "Friend")
	FText inGameText = TEXTSTR("En Jeu");

	/* Text displayed when player is online */
	UPROPERTY(EditAnywhere, Category = "Friend")
	FText onlineText = TEXTSTR("En ligne");

public:
	/**
	 * Get friend button template
	 * @return TSubclassOf<UFriendButtonTemplate>
	 */
	FORCEINLINE TSubclassOf<UFriendButtonTemplate> GetFriendButtonTemplate() const { return friendButtonReference; }
	/**
	 * Get friend row template
	 * @return TSubclassOf<UFriendRowTemplate>
	 */
	FORCEINLINE TSubclassOf<UFriendRowTemplate> GetFriendRowTemplate() const { return friendRowReference; }

	/**
	 * Get style of button with friend is in game
	 * @return FButtonStyle
	 */
	FORCEINLINE FButtonStyle GetInGameStyle() const { return brushFriendInGame; }
	/**
	 * Get style of button with friend is online
	 * @return FButtonStyle
	 */
	FORCEINLINE FButtonStyle GetOnlineStyle() const { return brushFriendOnline; }

	/**
	 * Get number of friend by row
	 * @return int
	 */
	FORCEINLINE int GetNumberFriendPerRow() const { return numberFriendPerRow; }

	/**
	 * Get text of friend if is online
	 * @return FString
	 */
	FORCEINLINE FString OnlineText() const { return onlineText.ToString(); }
	/**
	 * Get text of friend if is in game
	 * @return FString
	 */
	FORCEINLINE FString InGameText() const { return inGameText.ToString(); }
};
