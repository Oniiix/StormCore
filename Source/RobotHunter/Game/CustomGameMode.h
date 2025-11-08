#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CustomGameInstance.h"
#include "RobotHunter/Network/CustomGameSession.h"
#include "CustomGameMode.generated.h"

#define CUSTOM_GAMEMODE_USE_LOG true


USTRUCT()
struct FSpecificLevelsPerDay
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TMap<int, TSoftObjectPtr<UWorld>> levelsPerDay;
};



UCLASS()
class ROBOTHUNTER_API ACustomGameMode: public AGameMode
{
	GENERATED_BODY()



protected:
	UPROPERTY(Transient)
	TObjectPtr<ASessionSetup> sessionSetup = nullptr;
	

	UPROPERTY(EditAnywhere, Category = "Day")
	TMap<FString, FSpecificLevelsPerDay> customSubLevels;

public:
	ACustomGameMode();

	FORCEINLINE TObjectPtr<AGameSession> GetGameSession() { return GameSession; }


	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
protected:
	virtual void Init();

	virtual void GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList);

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	void LoadArenaLevel();
};
