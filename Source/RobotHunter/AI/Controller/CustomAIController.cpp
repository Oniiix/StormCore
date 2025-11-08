// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Navigation/PathFollowingComponent.h"

void ACustomAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	switch (Result.Code)
	{
	case EPathFollowingResult::Aborted:
		LOG_COLOR(AI_CONTROLLER_USE_DEBUG, "Path Aborted", ORANGE);
		break;
	case EPathFollowingResult::Blocked:
		LOG_COLOR(AI_CONTROLLER_USE_DEBUG, "Path Blocked", ORANGE);
		break;
	case EPathFollowingResult::Invalid:
		LOG_COLOR(AI_CONTROLLER_USE_DEBUG, "Path Invalid", ORANGE);
		break;
	case EPathFollowingResult::OffPath:
		LOG_COLOR(AI_CONTROLLER_USE_DEBUG, "Path OffPath", ORANGE);
		break;
	case EPathFollowingResult::Success:
		LOG_COLOR(AI_CONTROLLER_USE_DEBUG, "Reached destination", GREEN);
		onReachedDestination.Broadcast();
		break;
	}
}
