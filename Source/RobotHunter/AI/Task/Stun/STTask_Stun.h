// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_Stun.generated.h"

/**
 * Task that handle stun
 */
UCLASS()
class ROBOTHUNTER_API USTTask_Stun : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	float stunTime = 1.f;

	bool endTimerStun = false;

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	UFUNCTION()
	void EndTimerStun();
};
