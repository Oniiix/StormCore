// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "STTask_SearchMove.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_SearchMove : public USTTask_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	FVector searchPosition;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
