// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomGameInstance.h"
#include <RobotHunter/Game/Managers/Weather/WeatherManager.h>
#include "RobotHunter/Utils/ENUM_Speed.h"
#include "RobotHunter/Game/Managers/Handcar/StopHandcarManager.h"
#include "CustomGameState.generated.h"

/**
 * 
 */
class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API ACustomGameState : public AGameState
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxPlayerCountReached);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerQuit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoin, AMainCharacter*, _playerJoin);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeteoInit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAtmosphereInit);


	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
		FOnMeteoInit onMeteoInit;
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
		FOnAtmosphereInit onAtmosphereInit;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
		FOnPlayerJoin onPlayerJoin;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
		FOnPlayerQuit onPlayerQuit;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnMaxPlayerCountReached onMaxPlayerCountReached;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
		TEnumAsByte<EMeteoStatus> currentMeteo;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
		TObjectPtr<AWeather> currentWeather = nullptr;

	UPROPERTY(ReplicatedUsing= OnRep_UpdateWeatherManager)
		TObjectPtr<AWeatherManager> currentWeatherManager;


	//TODO Remove si on l'utilise plus
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AMainCharacter> playerOwner;

	UPROPERTY(VisibleAnywhere)
		TArray<AMainCharacter*> players;
	//

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ESpeedModifiers>, float> speedModifiers;
	

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EGameStatus> startStatus = MAINMENU;


	UPROPERTY()
		TObjectPtr<UStopHandcarManager> stopHandcarManager = nullptr;

	bool isInGame = false;

public:
	//EVENT
	FORCEINLINE TEnumAsByte<EGameStatus> GetStartStatus() const { return startStatus; }
	FORCEINLINE FOnPlayerJoin& OnPlayerJoin() { return onPlayerJoin; }
	FORCEINLINE FOnPlayerQuit& OnPlayerQuit() { return onPlayerQuit; }
	FORCEINLINE FOnAtmosphereInit& OnAtmosphereInit() { return onAtmosphereInit; }
	FORCEINLINE FOnMaxPlayerCountReached& OnMaxPlayerCountReached() { return onMaxPlayerCountReached; }

	//GETTER/SETTER [METEO]
	FORCEINLINE void SetWeatherManager(AWeatherManager* _weatherManager) { currentWeatherManager = _weatherManager; }
	FORCEINLINE AWeatherManager* GetWeatherManager() const { return currentWeatherManager; }

	void SetCurrentMeteo(AWeather* _weather);
	FORCEINLINE EMeteoStatus GetCurrentMeteo() const { return currentMeteo; }
	FORCEINLINE AWeather* GetCurrentWeather() const { return currentWeather; }

	//GETTER [PLAYERS]
	FORCEINLINE AMainCharacter* GetPlayerOwner() const { return playerOwner; }
	FORCEINLINE TArray<AMainCharacter*> GetPlayers() const { return players; } /* Si c'est le serveur qui pointe la liste, le character [0] = owner et [1] = autre client */
	FORCEINLINE TMap<TEnumAsByte<ESpeedModifiers>, float> GetSpeedModifiers() const { return speedModifiers; }

	//GETTER/SETTER [HANDCAR]
	FORCEINLINE TObjectPtr<UStopHandcarManager> GetHandcarManager() const { return stopHandcarManager; }
	FORCEINLINE void SetHandcarManager(UStopHandcarManager* _manager) { stopHandcarManager = _manager; }

	FORCEINLINE bool IsInGame() const { return isInGame; }

	ACustomGameState();

	//TODO Remove si inutile
	bool Register(AMainCharacter* _player, const bool _isOwner);
	bool Unregister(AMainCharacter* _player);
	//


	virtual void PostInitializeComponents() override;
protected:

//ONLINE
#pragma region Online
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UFUNCTION() void OnRep_UpdateWeatherManager();
#pragma endregion
};
