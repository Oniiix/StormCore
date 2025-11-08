// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "STTask_Escape.generated.h"

#define TASK_ESCAPE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTTask_Escape : public USTTask_Movement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<AActor> player = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float distance = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector newDirection;
	FVector test;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

private:
	/// <summary>
	/// Get an Escape Position
	/// </summary>
	/// <param name="_actorLocation"></param>
	/// <param name="_playerLocation"></param>
	/// <returns></returns>
	FVector EscapePosition(const FVector& _actorLocation, const FVector& _playerLocation);
};
