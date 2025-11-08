

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_IsBlocked.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_IsBlocked : public USTTask
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float minDistTest = 100;
	UPROPERTY(EditAnywhere)
	float timeStartTestDistance = 1;
	UPROPERTY(EditAnywhere)
	float timeAIIsBlock = 1;

	bool isBlocked = false;

	FTimerHandle timerTestPosition;

	FVector lastPosition;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	void TestDistanceIA();

	void AIIsBlocked();

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

};
