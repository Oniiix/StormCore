#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_Spawn.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_Spawn : public USTTask
{
	GENERATED_BODY()
	
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
