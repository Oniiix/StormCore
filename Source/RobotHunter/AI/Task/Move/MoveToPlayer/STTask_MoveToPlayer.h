// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "STTask_MoveToPlayer.generated.h"

#define TASK_MOVE_TO_PLAYER_USE_LOG false

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API USTTask_MoveToPlayer : public USTTask_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<AMainCharacter> target = nullptr;

	float countDeltaTime = 0;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
};
