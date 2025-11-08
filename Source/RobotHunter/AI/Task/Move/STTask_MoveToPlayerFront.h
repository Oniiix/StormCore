#pragma once

#include "CoreMinimal.h"
#include "STTask_Movement.h"
#include "STTask_MoveToPlayerFront.generated.h"

#define TASK_MOVE_FRONT_USE_LOG false

class AAI_Giant;

UCLASS()
class ROBOTHUNTER_API USTTask_MoveToPlayerFront : public USTTask_Movement
{
	GENERATED_BODY()

	TObjectPtr<AActor> target = nullptr;
	TObjectPtr<AActor> player = nullptr;
	TObjectPtr<AAI_Giant> titanOwner = nullptr;
	float countDeltaTime = 0;
	
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
};
