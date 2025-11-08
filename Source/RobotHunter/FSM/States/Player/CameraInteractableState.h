#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerState.h"
#include "CameraInteractableState.generated.h"

UCLASS()
class ROBOTHUNTER_API UCameraInteractableState : public UCustomPlayerState
{
	GENERATED_BODY()

private:
	void LinkCameraInteractableToFSM();
	void BlendEnded();

public:
	UFUNCTION() virtual void EnablePlayerContext() override;

protected:
	virtual void Enter() override;
	virtual void Exit() override;
};
