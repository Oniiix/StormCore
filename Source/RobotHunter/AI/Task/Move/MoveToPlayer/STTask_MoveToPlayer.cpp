// Fill out your copyright notice in the Description page of Project Settings.

#include "STTask_MoveToPlayer.h"
#include "RobotHunter/Utils/DebugUtils.h"

EStateTreeRunStatus USTTask_MoveToPlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (!target)
	{
		LOG_WARNING(TASK_MOVE_TO_PLAYER_USE_LOG, "target is null !!");
		return EStateTreeRunStatus::Failed;
	}
	movementComponent->Move(target, acceptanceRadius);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_MoveToPlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
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

