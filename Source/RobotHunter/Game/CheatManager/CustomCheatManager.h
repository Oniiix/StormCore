// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Menu/Pause/PauseMenu.h"
#include "RobotHunter/Utils/ENUM_UI.h"
#include "CustomCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
#pragma region UI
	
	/**
	 * Cheat command to set a ui without using menu
	 * @param _menu
	 * @param _subMenu
	 */
	UFUNCTION(Exec) void SetUI(const FString& _menu, const FString& _subMenu = "") const;
	
private:
	TFunction<void()> ShowOptions(const UPauseMenu* _pause) const;
	TFunction<void()> ShowGame(const UPauseMenu* _pause) const;
	TFunction<void()> ShowFriends(const UPauseMenu* _pause) const;
	TFunction<void()> ShowMap(const UPauseMenu* _pause) const;

#pragma endregion UI

#pragma region Player
	UFUNCTION(Exec) void AddLife(const int _lifeToAdd);
	UFUNCTION(Exec) void RemoveLife(const int _lifeToRemove);
	UFUNCTION(Exec) void TakePlayerDamage(int _idOfPlayer, int _damage);
#pragma endregion Player
};
