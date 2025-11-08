

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerManager.generated.h"

class AMainCharacter;

#define PLAYER_MANAGER_USE_LOG false


UCLASS(Blueprintable)
class ROBOTHUNTER_API UPlayerManager : public UObject
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPlayerJoin);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllwoPrivateAccess))
	FOnAllPlayerJoin onAllPlayerJoin;

	int controllerCount = 0;

	UPROPERTY()
	TArray<AMainCharacter*> players;

public:
	FORCEINLINE FOnAllPlayerJoin& OnAllPlayerJoin() { return onAllPlayerJoin; }

	FORCEINLINE TArray<AMainCharacter*> GetAllPlayers() const { return players; }
	void Init();

private:
	UFUNCTION() void PlayerJoin(AMainCharacter* _chara);
};
