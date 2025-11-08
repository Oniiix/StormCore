

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "STTask_MoveToRandomPointOutRange.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_MoveToRandomPointOutRange : public USTTask_Movement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float outRange = 500; 
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERangeAttack> range;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

};
