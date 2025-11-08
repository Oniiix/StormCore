// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "STTask_RandomMovement.generated.h"

#define TASK_RANDOM_MOVE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTTask_RandomMovement : public USTTask_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector newLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float radius;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

private:
	/// <summary>
	/// Get a RandomLocation 
	/// </summary>
	/// <param name="_radius"> radius circle </param>
	/// <param name="_origin"></param>
	/// <returns></returns>
	FVector RandomLocation(float _radius, FVector _origin, FStateTreeExecutionContext& Context);
};
