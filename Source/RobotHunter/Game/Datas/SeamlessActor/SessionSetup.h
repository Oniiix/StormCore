

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SessionSetup.generated.h"

USTRUCT()
struct FSessionInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TMap<FUniqueNetIdRepl, APlayerController*> playerList;

	UPROPERTY()
	TArray<FUniqueNetIdRepl> banList;
};


UCLASS()
class ROBOTHUNTER_API ASessionSetup : public AInfo
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	FSessionInfo sessionInfo;

public:
	FORCEINLINE FSessionInfo& GetSessionInfo() { return sessionInfo; }
	ASessionSetup();
};
