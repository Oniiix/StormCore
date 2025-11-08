// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_DisableCamouflage.generated.h"

#define TASK_DISABLE_CAMOUFLAGE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTTask_DisableCamouflage : public USTTask
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// Hidden camouflage Component
	/// </summary>
	/// <param name="Context"></param>
	/// <param name="Transition"></param>
	/// <returns></returns>
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
