// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_MoveToLocation.h"
#include <NavigationSystem.h>

EStateTreeRunStatus USTTask_MoveToLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	newLocation = GetNavigableLocationInArena(newLocation);

	movementComponent->Move(newLocation, acceptanceRadius);

	return EStateTreeRunStatus::Running;
}

void USTTask_MoveToLocation::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
