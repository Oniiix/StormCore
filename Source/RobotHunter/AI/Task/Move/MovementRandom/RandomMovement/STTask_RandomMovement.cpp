// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_RandomMovement.h"
#include "RobotHunter/AI/Component/Perception/PerceptionSystemComponent.h"
#include <NavigationSystem.h>
#include "RobotHunter/Utils/DebugUtils.h"

EStateTreeRunStatus USTTask_RandomMovement::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	newLocation = RandomLocation(radius, owner->GetActorLocation(), Context);
	if (!movementComponent)
	{
		LOG_ERROR(TASK_RANDOM_MOVE_USE_LOG, "MovementComponent of " + GetOwnerActor(Context)->GetActorNameOrLabel() + " is null !!");
		return EStateTreeRunStatus::Failed;
	}
	movementComponent->Move(newLocation, acceptanceRadius);
	return EStateTreeRunStatus::Running;
}

FVector USTTask_RandomMovement::RandomLocation(float _radius, FVector _origin, FStateTreeExecutionContext& Context)
{
	return GetRandomNavigableLocation(_origin, _radius);
}

void USTTask_RandomMovement::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
