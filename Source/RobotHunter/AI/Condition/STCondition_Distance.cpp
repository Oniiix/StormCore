// Fill out your copyright notice in the Description page of Project Settings.


#include "STCondition_Distance.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "StateTreeExecutionContext.h"

bool USTCondition_Distance::TestCondition(FStateTreeExecutionContext& Context) const
{
	Super::TestCondition(Context);
	if (!actor)
	{
		LOG_ERROR(CONDITION_DISTANCE_USE_LOG, "actor is null!!");
		return false;
	}
	if (debugMode)
	{
		DrawDebugLine(GetWorld(), actor->GetActorLocation(), newLocation, FColor::Red);
		LOG_WARNING(CONDITION_DISTANCE_USE_LOG, FString::SanitizeFloat(FVector::Distance(actor->GetActorLocation(), newLocation)));

	}
	return TestOperator();
}

bool USTCondition_Distance::TestOperator() const
{
	switch (operatorC)
	{
	case LESS:
		return FVector::DistXY(actor->GetActorLocation(), newLocation) < distance;
	case LESS_OR_EQUAL:
		return FVector::DistXY(actor->GetActorLocation(), newLocation) <= distance;
	case EQUAL:
		return FVector::DistXY(actor->GetActorLocation(), newLocation) == distance;
	case NOT_EQUAL:
		return FVector::DistXY(actor->GetActorLocation(), newLocation) != distance;
	case GREATER_OR_EQUAL:
		return FVector::DistXY(actor->GetActorLocation(), newLocation) >= distance;
	case GREATER:
		return FVector::DistXY(actor->GetActorLocation(), newLocation) > distance;
	default:
		return false;
	}
	
}
