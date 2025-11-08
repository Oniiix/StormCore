#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/FSM/Transitions/Player/CustomPlayerTransition.h"
#include "ToSprintTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToSprintTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()
public:
	virtual bool IsValidTransition() override;
};
