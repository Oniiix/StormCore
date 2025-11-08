#pragma once

#include "CoreMinimal.h"
#include "Components/Overlay.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"
#include "RobotHunter/UI/Template/MainMenu/Host/ButtonSaveTemplate.h"
#include "RobotHunter/UI/Template/Prefab/ScrollBoxPrefab/ScrollboxPrefab.h"
#include "RobotHunter/UI/Template/Prefab/SelectorPrefab/SelectorPrefab.h"
#include "HostMenu.generated.h"

#define HOST_MENU_USE_LOG false

class UPopupData;
class UButtonCreateSaveTemplate;
class USaveManagerSubsystem;
class UHostSettings;

UCLASS(HideDropdown)
class ROBOTHUNTER_API UHostMenu final : public UBaseMenu
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlay, int, saveSlot);

	// --- UPROPERTY ---
	UPROPERTY()
	FOnPlay onPlay;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> backButton;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UScrollboxPrefab> saveList;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<USelectorPrefab> SessionSelector;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UOverlay> OverlaySelector;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UHostSettings> settings;

	UPROPERTY()
	USaveManagerSubsystem* saveManager;

	UPROPERTY()
	UCustomGameInstance* GameInstance;

	UPROPERTY()
	TObjectPtr<UButtonPrefab> addButton;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UPopupData> popupIntro;
	UPROPERTY(EditAnywhere, Category = "Custom")
	FName worldIntro;

	UPROPERTY()
	int creatingSlot;

public:
	FORCEINLINE FOnPlay& OnPlay() { return onPlay; }

	UFUNCTION()
	FORCEINLINE void Callback_OnPlay(int _slot) const { onPlay.Broadcast(_slot); }

	virtual void Refresh() override;
	
protected:
	virtual void Init() override;
	UFUNCTION()
	void Launch(int SaveSlot);
	virtual void RegisterEvents() override;

	virtual void DisableAllWidgetInteractable() override;

private:
	//virtual void HandleInputChanged(bool isGamepad) override;

	
	UFUNCTION()
	void LaunchMap();

	/**
	* Create a new session and travel
	*/
	UFUNCTION()
	void Create(const int _slot);

	/**
	* Go back to the main menu
	*/
	UFUNCTION()
	void Back();

	/** Create save game */
	UFUNCTION()
	void CreateSaveGame();

	/**
	* Load all saves
	*/
	void LoadSaves();

	void CallPopup() const;
	void SaveCurrentSlot(bool _isTutorial);

	UFUNCTION()
	void LaunchMapTutorial();
	UFUNCTION(BlueprintCallable, Category = "Custom|Host")
	void PlayTutorial();
	UFUNCTION(BlueprintCallable, Category = "Custom|Host")
	void SkipTutorial();
};
