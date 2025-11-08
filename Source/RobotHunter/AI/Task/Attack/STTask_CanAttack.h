#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_CanAttack : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float maxAttackHeight = 200.f;
	
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
