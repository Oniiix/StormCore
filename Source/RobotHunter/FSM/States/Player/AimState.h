#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerState.h"
#include "AimState.generated.h"

class ARightArmActor;

UCLASS()
class ROBOTHUNTER_API UAimState : public UCustomPlayerState
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<ARightArmActor> rightArm;

public:
	UAimState();

private:
	void RetrieveRightArm();

public:
	virtual void Enter() override;
};
