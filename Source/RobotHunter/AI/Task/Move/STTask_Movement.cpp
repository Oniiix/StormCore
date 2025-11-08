// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_Movement.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include <NavigationSystem.h>
#include "RobotHunter/Utils/DebugUtils.h"

EStateTreeRunStatus USTTask_Movement::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	movementComponent = owner->GetComponentByClass<UAIMovementComponent>();
	if (!movementComponent)
	{
		LOG_ERROR(TASK_MOVE_USE_LOG, "Failed to get AIMovementComponent on " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	movementComponent->UpdateSpeed(runMode);

	return EStateTreeRunStatus::Running;
}

FVector USTTask_Movement::GetNavigableLocation(const FVector& _targetLocation)
{
	positionIsNaviagble = true;
	UNavigationSystemV1* _nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!_nav)
	{
		LOG_ERROR(TASK_MOVE_USE_LOG, "Failed to find nav system on " + owner->GetActorNameOrLabel());
		return owner->GetActorLocation();
	}
	FNavLocation _navLocation;
	if (!_nav->ProjectPointToNavigation(_targetLocation, _navLocation))
	{
		LOG_TICK(true, "Failed to get navigable point");
		positionIsNaviagble = false;
		return owner->GetActorLocation();
	}

	return _navLocation.Location;
}

FVector USTTask_Movement::GetRandomNavigableLocation(const FVector& _targetLocation, const float _radius)const
{
	UNavigationSystemV1* _nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!_nav)
	{
		LOG_ERROR(TASK_MOVE_USE_LOG, "Failed to find nav system on " + owner->GetActorNameOrLabel());
		return owner->GetActorLocation();
	}
	FNavLocation _navLocation;
	if (!_nav->GetRandomReachablePointInRadius(_targetLocation, _radius, _navLocation))
	{
		LOG_TICK(true, "Failed to get navigable point");
		return owner->GetActorLocation();
	}

	return _navLocation.Location;
}

FVector USTTask_Movement::GetNavigableLocationInArena(const FVector& _targetLocation)
{
	targetLocation = _targetLocation;
	const AArena* _arena = owner->GetArena();
	const FVector& _arenaLocation = _arena->GetActorLocation();
	const float _arenaRange = _arena->GetBreakRange();

	const float _distanceArenaToTarget = FVector::Distance(_arenaLocation, _targetLocation);

	const FVector& _direction = (_targetLocation - _arenaLocation).GetSafeNormal();

	FVector _position;
	if (_distanceArenaToTarget <= _arenaRange)
	{
		targetInArena = true;
		_position = _targetLocation;
	}
	else
	{
		targetInArena = false;
		_position += _direction * _arenaRange;
	}
	targetLocation = _position;
	return GetNavigableLocation(_position);
}

void USTTask_Movement::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	if (!useStopMove)
		return;
	if (!movementComponent)
		return;
	movementComponent->StopMove();
}
