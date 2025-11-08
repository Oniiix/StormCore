
#pragma once

#include "CoreMinimal.h"
#include "../CustomGameMode.h"
#include "RobotHunter/AI/Arena/ArenaManager.h"
#include "RobotHunter/Game/TEST/PlayerManager.h"
#include "PrincipalGameMode.generated.h"

class ULightningGenerator;


#define PRINCIPAL_GAMEMODE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API APrincipalGameMode : public ACustomGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Arena")
	TSubclassOf<UArenaManager> arenaManagerRef = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TObjectPtr<UArenaManager> arenaManager = nullptr;

	UPROPERTY()
	TObjectPtr<UPlayerManager> playerManager = nullptr;

	FTimerHandle timerDelayedInit;
	int32 numberOfPlayer = 0;

public:
	UFUNCTION(BlueprintPure) FORCEINLINE UArenaManager* GetArenaManager() const { return arenaManager; }

	FORCEINLINE TObjectPtr<UPlayerManager> GetPlayerManager() const { return playerManager; }

	virtual void PreInitializeComponents() override;

protected:
	virtual void Init();

	UFUNCTION()
	void DelayedInit();

	virtual void InitGameState() override;
};
