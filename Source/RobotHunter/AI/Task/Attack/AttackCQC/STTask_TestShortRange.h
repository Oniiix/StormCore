
#pragma once
#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_TestShortRange.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_TestShortRange : public USTTask
{
	GENERATED_BODY()

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
