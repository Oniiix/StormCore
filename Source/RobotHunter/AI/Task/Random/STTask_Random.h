// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_Random.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_Random : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0, UMax = 100, ClampMax = 100))
	int chancePercent = 25;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
