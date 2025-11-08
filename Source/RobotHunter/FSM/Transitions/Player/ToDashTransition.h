#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/FSM/Transitions/Player/CustomPlayerTransition.h"
#include "ToDashTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToDashTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition() override;
};
