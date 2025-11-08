// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_Random.h"

EStateTreeRunStatus USTTask_Random::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	const int _res = FMath::RandRange(0, 100);
	if (_res <= chancePercent)
		return EStateTreeRunStatus::Succeeded;
	else
		return EStateTreeRunStatus::Failed;
}
