// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GPE/Weathers/Weather.h"
#include "Managers/Quest/QuestManager.h"
#include "RobotHunter/UI/ATH/Game/Cooldown/KeyImageDataAsset.h"
#include "CustomGameInstance.generated.h"

UENUM(Blueprintable, BlueprintType)
enum EGameStatus
{
	GameNone,
	MAINMENU,
	HUB,
	EXPLORATION,
	STEALTH,
	FIGHTING,
	SHOOTING,

	COUNT,
};

UENUM(Blueprintable, BlueprintType)
enum class ETypeChange : uint8
{
	MainToHub,
	HubToGame,
	HubToMain,
	GameToHub,
	GameToMain,
	Quit,
	DeathToHub,
	MainToIntro,
	IntroToHub,
	IntroToMain,
};

UCLASS()
class ROBOTHUNTER_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherChange, TSubclassOf<AWeather>, _weather);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeMainToHub);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeHubToGame);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeHubToMain);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeGameToHub);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeGameToMain);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeMainToQuit);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeMainToIntro);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeIntroToHub);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeIntroToMain);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathToHub);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoad);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoaded);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishFadeOut);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishFadeIn);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnding);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndingCredit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndingCreditClose);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFPSShow, bool, onFPSShow);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLanguageChange, const FString, _shortcut);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeMainToHub onLevelChangeMainToHub;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeHubToGame onLevelChangeHubToGame;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeHubToMain onLevelChangeHubToMain;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeGameToHub onLevelChangeGameToHub;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeGameToMain onLevelChangeGameToMain;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeMainToQuit onLevelChangeMainToQuit;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeMainToQuit onDeathToHub;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelLoad onLevelLoad;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelLoaded onLevelLoaded;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnFinishFadeOut onFinishFadeOut;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnFinishFadeIn onFinishFadeIn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeMainToIntro onLevelChangeMainToIntro;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeIntroToHub onLevelChangeIntroToHub;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLevelChangeIntroToMain onLevelChangeIntroToMain;


	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnFPSShow fpsShow;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLanguageChange onLanguageChange;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Custom|Event|Ending", meta = (AllowPrivateAccess))
	FOnEnding onEndingTriggered;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Custom|Event|Ending", meta = (AllowPrivateAccess))
	FOnEndingCredit onEndingCredit;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Custom|Event|Ending", meta = (AllowPrivateAccess))
	FOnEndingCreditClose onEndingCreditClose;


	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnWeatherChange onWeatherChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta =(AllowPrivateAccess))
	TEnumAsByte<EGameStatus> status = MAINMENU;

	UPROPERTY()
	bool isOnlineMode = false;
	UPROPERTY()
	bool isInTutorial = false;
	UPROPERTY()
	bool isInPause = false;
	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	bool isInSequencer = false;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> mainMenuLevel = nullptr;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> hubLevel = nullptr;

#pragma region Quests
	UPROPERTY()
	TObjectPtr<AQuestManager> questManager = nullptr;
#pragma endregion

#pragma region METEO

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeather> currentWeather;
#pragma endregion


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	TObjectPtr<UKeyImageDataAsset> keyImageDataAsset = nullptr;

public:
	virtual void Init() override;

	FORCEINLINE FOnLevelLoad& OnLevelLoad() { return onLevelLoad; }
	FORCEINLINE FOnLevelLoaded& OnLevelLoaded() { return onLevelLoaded; }
	FORCEINLINE FOnFinishFadeOut& OnFinishFadeOut() { return onFinishFadeOut; }	
	FORCEINLINE FOnFPSShow& OnFPSShow() { return fpsShow; } 
	FORCEINLINE FOnFinishFadeIn& OnFinishFadeIn() { return onFinishFadeIn; }
	void CallbackOnLevelChange(const ETypeChange _type) const;
	void CallbackOnLanguageChange(const FString& _shortcut) const;


	FORCEINLINE void SetOnlineMode(const bool _active) { isOnlineMode = _active; }
	FORCEINLINE bool GetOnlineMode() const { return isOnlineMode; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetInTutorial(const bool _active) { isInTutorial = _active; }

	FORCEINLINE bool GetInTutorial() const { return isInTutorial; }

	FORCEINLINE void SetInPause(const bool _active) { isInPause = _active; }
	FORCEINLINE bool GetInPause() const { return isInPause; }


	FORCEINLINE EGameStatus GetGameStatus() const { return status; }
	FORCEINLINE void SetGameStatus(const EGameStatus _status) { status = _status; }

	FORCEINLINE TSoftObjectPtr<UWorld> GetHubLevel() { return hubLevel; }
	FORCEINLINE void SetHubLevel(const TSoftObjectPtr<UWorld>& _level) { hubLevel = _level; }

	FORCEINLINE TSoftObjectPtr<UWorld> GetMainMenuLevel() { return mainMenuLevel; }
	FORCEINLINE void SetMainMenuLevel(const TSoftObjectPtr<UWorld>& _level) { mainMenuLevel = _level; }

	FORCEINLINE TSubclassOf<AWeather> GetCurrentWeather() { return currentWeather; }
	FORCEINLINE TObjectPtr<AQuestManager> GetQuestManager() { return questManager; }
	FORCEINLINE void SetQuestManager(AQuestManager* _questManager) { questManager = _questManager; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentWeather(const TSubclassOf<AWeather>& _weather)
	{
		currentWeather = _weather;
		onWeatherChange.Broadcast(currentWeather);
	}

	FORCEINLINE FOnWeatherChange& OnWeatherChange() { return onWeatherChange; }
};
