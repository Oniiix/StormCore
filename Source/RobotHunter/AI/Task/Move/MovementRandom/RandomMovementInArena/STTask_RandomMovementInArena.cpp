// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_RandomMovementInArena.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <NavigationSystem.h>

EStateTreeRunStatus USTTask_RandomMovementInArena::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	AAI_Base* _owner = Cast<AAI_Base>(GetOwnerActor(Context));
	if (!_owner)
	{
		LOG_ERROR(TASK_RANDOM_MOVE_IN_ARENA_USE_LOG, "Failed to cast owner on " + GetFName().ToString());
		return EStateTreeRunStatus::Failed;
	}

	const FVector& _newLocation = RandomLocation(_owner);
	movementComponent->Move(_newLocation, acceptanceRadius);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_RandomMovementInArena::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
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

FVector USTTask_RandomMovementInArena::RandomLocation(AAI_Base* _AI)
{
	//AArena* _arena = _AI->GetArena();
	//if (!_arena)
	//{
	//	LOG_ERROR(TASK_RANDOM_MOVE_IN_ARENA_USE_LOG, "Arena of " + _AI->GetActorNameOrLabel() + " is null !!");
	//	return _AI->GetActorLocation();
	//}
	//return _arena->RandomLocationInArena();

	FVector _randomLocation = owner->GetStartLocation();
	const int _AIRandomMoveRadius = _AI->GetRandomMoveRadius();
	/// Check NavigationSystem availability
	UNavigationSystemV1* _nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!_nav)
	{
		LOG_ERROR(ARENA_USE_LOG, "Failed to find navigation system on " + owner->ACTOR_NAME);
		return _randomLocation;
	}
	/// Get available location
	FNavLocation _point;
	for (int i = 0; i < 100; i++)
	{
		const bool _success = _nav->GetRandomReachablePointInRadius(_randomLocation, _AIRandomMoveRadius, _point);
		if (_success)
		{
			_randomLocation = _point.Location;
			break;
		}
	}

	return _randomLocation;
}
