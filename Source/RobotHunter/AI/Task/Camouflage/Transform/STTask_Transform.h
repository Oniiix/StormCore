// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_Transform.generated.h"

#define TASK_TRANSFORM_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTTask_Transform : public USTTask
{
	GENERATED_BODY()

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
