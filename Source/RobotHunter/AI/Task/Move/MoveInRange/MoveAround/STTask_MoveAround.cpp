
#include "STTask_MoveAround.h"

EStateTreeRunStatus USTTask_MoveAround::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	const FVector& _newLocation = arenaRangeComponent->GetPositionRandomAround(point, length);

	movementComponent->Move(_newLocation, acceptanceRadius);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_MoveAround::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	/// Unlock if didn't move
	const float _ownerVelocity = owner->GetVelocity().Length();
	if (_ownerVelocity <= 1.f)
	{
		countDeltaTime += DeltaTime;
		if (countDeltaTime >= 0.1f)
			return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}
