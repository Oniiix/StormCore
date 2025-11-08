// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "RobotHunter/Game/Datas/SeamlessActor/SessionSetup.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "CustomGameSession.generated.h"

#define GAME_SESSION_USE_LOG false

UCLASS(Blueprintable)
class ROBOTHUNTER_API ACustomGameSession : public AGameSession
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FUniqueNetIdRepl, APlayerController*> playerList;

	UPROPERTY()
	TArray<FUniqueNetIdRepl> banList;

	UPROPERTY()
	TObjectPtr<ASessionSetup> sessionSetup = nullptr;

#pragma region setter/getter
public:
	FORCEINLINE TMap<FUniqueNetIdRepl, APlayerController*> GetPlayerList() { return sessionSetup->GetSessionInfo().playerList; }
	FORCEINLINE void SetSessionSetup(ASessionSetup* _sessionSetup) { sessionSetup = _sessionSetup; }
#pragma endregion

private:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void NotifyLogout(const APlayerController* PC) override;

public:
	void RegisterPlayerController(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId);
	void UnRegisterPlayerController(const FUniqueNetIdRepl& UniqueId);
	void BanPlayerCustom(const FUniqueNetIdRepl& UniqueId);
	void KickPlayerCustom(APlayerController* _playerController);
};
