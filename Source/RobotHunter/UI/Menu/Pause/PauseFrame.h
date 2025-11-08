#pragma once

#include "CoreMinimal.h"
#include "PauseMenu.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"
#include "PauseFrame.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ROBOTHUNTER_API UPauseFrame : public UBaseMenu
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UPauseMenu> owner;

public:
	FORCEINLINE void SetOwner(UPauseMenu* PauseMenu) { owner = PauseMenu; }
};
