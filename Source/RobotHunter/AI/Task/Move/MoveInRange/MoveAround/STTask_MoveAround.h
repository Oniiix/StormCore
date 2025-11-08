
#pragma once
#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "STTask_MoveAround.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_MoveAround : public USTTask_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float length = 0;
	UPROPERTY(EditAnywhere)
	FVector point;

	float countDeltaTime = 0;

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
};
