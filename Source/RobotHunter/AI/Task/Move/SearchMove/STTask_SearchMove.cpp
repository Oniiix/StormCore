// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_SearchMove.h"
#include "RobotHunter/AI/AI_Scout/AI_Scout.h"

EStateTreeRunStatus USTTask_SearchMove::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	searchPosition = GetNavigableLocationInArena(searchPosition);
	movementComponent->Move(searchPosition);

	return EStateTreeRunStatus::Running;
}

void USTTask_SearchMove::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	AAI_Scout* _scout = Cast<AAI_Scout>(owner);
	_scout->SetIsEndSearch(FVector::Distance(_scout->GetActorLocation(), searchPosition) <= 50);
}
