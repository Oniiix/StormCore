#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerState.h"
#include "MenuState.generated.h"

#define MENU_STATE_USE_LOG false


UCLASS()
class ROBOTHUNTER_API UMenuState : public UCustomPlayerState
{
	GENERATED_BODY()

private:
	void EnterMenuHUBState();
	void BlendEnded();

public:
	UFUNCTION() virtual void EnablePlayerContext() override;
	
protected:
	virtual void Enter() override;
	virtual void Exit() override;
};
