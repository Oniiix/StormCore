

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_SelectRange.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_SelectRange : public USTTask
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Out")
	TEnumAsByte<ERangeAttack> rangeToMove;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;
};
