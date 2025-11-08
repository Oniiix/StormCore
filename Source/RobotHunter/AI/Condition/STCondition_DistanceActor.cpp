// Fill out your copyright notice in the Description page of Project Settings.


#include "STCondition_DistanceActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

bool USTCondition_DistanceActor::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (!player)
	{
		LOG_ERROR(CONDITION_DISTANCE_ACTOR_USE_LOG, "player is null!!");
		return false;
	}
	return TestOperator();
}

bool USTCondition_DistanceActor::TestOperator() const
{
	switch (operatorC)
	{
	case LESS:
		return FVector::Distance(actor->GetActorLocation(), player->GetActorLocation()) < distance;
	case LESS_OR_EQUAL:
		return FVector::Distance(actor->GetActorLocation(), player->GetActorLocation()) <= distance;
	case EQUAL:
		return FVector::Distance(actor->GetActorLocation(), player->GetActorLocation()) == distance;
	case NOT_EQUAL:
		return FVector::Distance(actor->GetActorLocation(), player->GetActorLocation()) != distance;
	case GREATER_OR_EQUAL:
		return FVector::Distance(actor->GetActorLocation(), player->GetActorLocation()) >= distance;
	case GREATER:
		return FVector::Distance(actor->GetActorLocation(), player->GetActorLocation()) > distance;
	default:
		return false;
	}
}
