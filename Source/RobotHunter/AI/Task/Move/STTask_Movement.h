// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "STTask_Movement.generated.h"

#define TASK_MOVE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTTask_Movement : public USTTask
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UAIMovementComponent> movementComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float acceptanceRadius = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool runMode = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool useStopMove = true;

	FVector targetLocation;

	bool targetInArena = true;
	bool positionIsNaviagble = true;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	/// <summary>
	/// Stop all Movement end task
	/// </summary>
	/// <param name="Context"></param>
	/// <param name="Transition"></param>
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/// <summary>
	/// Get closest reachable point in navMesh 
	/// </summary>
	/// <param name="_targetLocation"> location to reach </param>
	/// <returns> a reachable location </returns>
	FVector GetNavigableLocation(const FVector& _targetLocation);
	/// <summary>
	/// Get random reachable point in navMesh in radius
	/// </summary>
	/// <param name="_targetLocation"> The location to reach </param>
	/// <param name="_radius"> The radius </param>
	/// <returns> A reachable location </returns>
	FVector GetRandomNavigableLocation(const FVector& _targetLocation, const float _radius)const;
	/// <summary>
	/// Get closest reachable point in navMesh in arena
	/// </summary>
	/// <param name="_targetLocation"> location to reach </param>
	/// <returns> a reachable location </returns>
	FVector GetNavigableLocationInArena(const FVector& _targetLocation);
};
