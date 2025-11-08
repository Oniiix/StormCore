// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "STTask_Debug.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_Debug : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText text;
	UPROPERTY(EditAnywhere)
	FColor color = FColor::White;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
