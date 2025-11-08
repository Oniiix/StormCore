

#pragma once

#include "CoreMinimal.h"
#include "../CustomGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ALobbyGameMode : public ACustomGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PreInitializeComponents() override;
	virtual void StartToLeaveMap() override;

	virtual void Init() override;
};
