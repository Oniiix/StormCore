// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_Escape.h"
#include "RobotHunter/AI/AI_Scout/AI_Scout.h"

EStateTreeRunStatus USTTask_Escape::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (!player)
	{
		LOG_ERROR(TASK_ESCAPE_USE_LOG, "player is null !!");
		return EStateTreeRunStatus::Failed;
	}

	newDirection = GetNavigableLocationInArena(EscapePosition(owner->GetActorLocation(), player->GetActorLocation()));
	movementComponent->Move(newDirection);

	AAI_Scout* _scout = Cast<AAI_Scout>(owner);
	_scout->SetCanEscape(false);
	_scout->SetSearchPosition(player->GetActorLocation());
	return EStateTreeRunStatus::Running;
}

FVector USTTask_Escape::EscapePosition(const FVector& _actorLocation, const FVector& _playerLocation)
{
	FVector _direction = _playerLocation - _actorLocation;
	_direction = -_direction.GetSafeNormal();
	const FVector& _newPosition = _actorLocation + (FVector(_direction.X, _direction.Y, 0) * distance);
	return _newPosition;
}

void USTTask_Escape::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
