

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_Jump.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_Jump : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float speedJump = 200;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float jumpHeight = 300;

	FVector maxHeightPosition;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime)override;
};
