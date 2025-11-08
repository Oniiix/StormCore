#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_CheckAI.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_CheckAI : public USTTask
{
	GENERATED_BODY()
	
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
