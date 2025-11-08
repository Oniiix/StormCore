#include "STTask_MoveToRandomPointOutRange.h"

EStateTreeRunStatus USTTask_MoveToRandomPointOutRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	const TArray<FPointRangeArena>& _listPoint = arenaRangeComponent->GetListPointInRange(range, AITrackingComponent->GetTarget());
	const FVector& _aiLocation = owner->GetActorLocation();

	FPointRangeArena _targetPoint;
	float _dist = TNumericLimits<int>::Max();

	for (FPointRangeArena _point : _listPoint)
	{
		if (!_point.isAvailable)
			continue;

		const float _distancePoint = FVector::Dist(_point.position, _aiLocation);
		if (_distancePoint < outRange)
			continue;

		if (_distancePoint < _dist)
		{
			_targetPoint = _point;
			_dist = _distancePoint;
		}
	}

	const FVector& _newLocation = GetNavigableLocationInArena(_targetPoint.position);
	movementComponent->SetTargetPoint(_targetPoint);

	movementComponent->Move(_newLocation, acceptanceRadius);
	return EStateTreeRunStatus::Running;
}

void USTTask_MoveToRandomPointOutRange::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
