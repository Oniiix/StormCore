

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/AI/Arena/Component/AIRangePosition/AIRangePositionComponent.h"
#include "STTask_MovementToPoint.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_MovementToPoint : public USTTask_Movement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FPointRangeArena point;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
