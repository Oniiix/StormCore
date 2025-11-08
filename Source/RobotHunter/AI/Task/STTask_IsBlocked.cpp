#include "STTask_IsBlocked.h"

EStateTreeRunStatus USTTask_IsBlocked::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	lastPosition = owner->GetActorLocation();

	owner->GetWorldTimerManager().SetTimer(timerTestPosition, this, &USTTask_IsBlocked::TestDistanceIA, timeStartTestDistance, true);

	return EStateTreeRunStatus::Running;
}

void USTTask_IsBlocked::TestDistanceIA()
{
	const FVector& _actorLocation = owner->GetActorLocation();
	if (FVector::Dist(lastPosition, _actorLocation) < minDistTest)
	{
		FTimerManager& _timerManager = owner->GetWorldTimerManager();
		_timerManager.ClearTimer(timerTestPosition);
		_timerManager.SetTimer(timerTestPosition, this, &USTTask_IsBlocked::AIIsBlocked, timeStartTestDistance);
	}
	lastPosition = _actorLocation;
}

void USTTask_IsBlocked::AIIsBlocked()
{
	const FVector& _actorLocation = owner->GetActorLocation();
	if (FVector::Dist(lastPosition, _actorLocation) < minDistTest)
	{
		isBlocked = true;
		return;
	}
	lastPosition = _actorLocation;
	owner->GetWorldTimerManager().SetTimer(timerTestPosition, this, &USTTask_IsBlocked::TestDistanceIA, timeStartTestDistance, true);
}

EStateTreeRunStatus USTTask_IsBlocked::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	if(isBlocked)
		return EStateTreeRunStatus::Succeeded;

	return EStateTreeRunStatus::Running;
}

void USTTask_IsBlocked::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	owner->GetWorldTimerManager().ClearTimer(timerTestPosition);
}
