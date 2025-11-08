

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_DisableJump.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_DisableJump : public USTTask
{
	GENERATED_BODY()
	
protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

};
