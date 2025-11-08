#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_InterceptJump.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_InterceptJump : public USTTask
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (Umin = 0, ClampMin = 0))
	float jumpDelay = 1.f;
	UPROPERTY(EditAnywhere, meta = (Umin = 0, ClampMin = 0, UMax = 100, ClampMax = 100))
	float jumpProbability = 50.f;
	UPROPERTY(EditAnywhere, meta = (Umin = 1, ClampMin = 1))
	float jumpPower = 3.f;
	UPROPERTY(EditAnywhere, meta = (Umin = 0.1, ClampMin = 0.1, UMax = 1.0, ClampMax = 1.0))
	float pushSnapPower = 0.5f;

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	UFUNCTION()
	void InterceptJump();
};
