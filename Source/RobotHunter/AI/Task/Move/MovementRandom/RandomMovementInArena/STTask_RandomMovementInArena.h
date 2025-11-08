// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "STTask_RandomMovementInArena.generated.h"

#define TASK_RANDOM_MOVE_IN_ARENA_USE_LOG false

/**
 * Move to a random location in arena around AI's start
 * Avoid to close location on nest if Arena_Nest
 */
UCLASS()
class ROBOTHUNTER_API USTTask_RandomMovementInArena : public USTTask_Movement
{
	GENERATED_BODY()

	float countDeltaTime = 0;

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	/// <summary>
	/// Get a random location in arena's AI
	/// If arena is a nest => avoid nest range
	/// </summary>
	/// <param name="_AI"> The current AI of stateTree </param>
	/// <returns> A location in arena </returns>
	FVector RandomLocation(AAI_Base* _AI);
};
