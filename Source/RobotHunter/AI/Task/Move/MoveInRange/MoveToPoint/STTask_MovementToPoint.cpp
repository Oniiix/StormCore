#include "STTask_MovementToPoint.h"

EStateTreeRunStatus USTTask_MovementToPoint::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	const FVector& _newLocation = GetNavigableLocationInArena(point.position);
	movementComponent->SetTargetPoint(point);

	movementComponent->Move(_newLocation, acceptanceRadius);

	return EStateTreeRunStatus::Running;
}

void USTTask_MovementToPoint::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
