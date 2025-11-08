// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_AttackCQC.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_AttackCQC : public USTTask
{
	GENERATED_BODY()
	
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
