#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_SelectMovement.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_SelectMovement : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0, UMax = 180, ClampMax = 180))
	float FOV = 30.f;
	
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
