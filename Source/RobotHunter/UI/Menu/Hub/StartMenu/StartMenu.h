#pragma once

#include "CoreMinimal.h"
#include "../../BaseMenu.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "StartMenu.generated.h"

#define START_MENU_USE_LOG false


class UStartMenuSettings;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UStartMenu final : public UBaseMenu
{
	GENERATED_BODY()

#pragma region properties


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> playButton;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> backButton;


	/** The settings of start menu */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Settings", meta = (AllowPrivateAccess))
	TObjectPtr<UStartMenuSettings> settings;


	UPROPERTY()
	TObjectPtr<UCustomGameInstance> gi = nullptr;


	FTimerHandle timerLoading;
#pragma endregion properties

#pragma region methods
#pragma region private
	UFUNCTION()
	void LoadMap();
	/**
	*Charge and teleport players to the level
	**/
	UFUNCTION()
	void Play();
	/** 
	* Back to HUB menu
	*/
	UFUNCTION()
	void Exit();

	virtual void Refresh() override;
	virtual void HandleInputChanged(bool isGamepad) override;
	/**
	 * Load level, start server and travel players
	 * @param _name Name of level
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom", Meta = (AllowPrivateAccess))
	void LoadLevel(const FString& _name);

#pragma endregion private
#pragma region protected

protected:
	virtual void RegisterEvents() override;
	virtual void Init() override;

#pragma endregion protected

private:
	virtual void DisableAllWidgetInteractable() override;
#pragma endregion methods
};
