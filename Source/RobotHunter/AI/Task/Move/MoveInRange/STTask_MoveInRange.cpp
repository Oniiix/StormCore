// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_MoveInRange.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

EStateTreeRunStatus USTTask_MoveInRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	player = AITrackingComponent->GetTarget();

	acceptanceRadius = arenaRangeComponent->GetAcceptanceRadiusAI();

	FVector _newLocation;
	if (arenaRangeComponent->PointIsValid(movementComponent->GetRangePoint(), typeRange, player))
	{
		LOG_COLOR(false, "Faut faire mieux son travail Antoni", CYAN);
		_newLocation = owner->GetStartLocation();
	}
	else
	{
		_newLocation = movementComponent->GetNewTargetPointRange(typeRange, player, true, offSetShoot);
		if (_newLocation == FVector::ZeroVector)
		{
			_newLocation = owner->GetStartLocation();
		}
	}

	owner->SetRange(typeRange);

	_newLocation = GetNavigableLocationInArena(_newLocation);

	movementComponent->Move(_newLocation, acceptanceRadius);
	movementComponent->SetCanMoveInRange(false);

	return EStateTreeRunStatus::Running;
}

void USTTask_MoveInRange::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	const FVector& _actorLocation = owner->GetActorLocation();
	if (!arenaRangeComponent->IsAIInOfRange(_actorLocation, player, typeRange))
		movementComponent->SetTargetPoint(FPointRangeArena());
	else
	{
		const FPointRangeArena& _point = arenaRangeComponent->GetPointToLocation(_actorLocation, typeRange, player);
		if (_point.index != movementComponent->GetRangePoint().index)
		{
			arenaRangeComponent->SetAvailablePoint(_point, false);
			movementComponent->SetTargetPoint(_point);
		}
	}
}
