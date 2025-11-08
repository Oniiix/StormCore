// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "STTask_ShootTongueProjectile.generated.h"

#define TASK_SHOOT_TONGUE_PROJECTILE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USTTask_ShootTongueProjectile : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<AMainCharacter> target = nullptr;

	FTimerHandle shootTimer;
	float indexDebug;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	UFUNCTION() void ShootTongue();
	void Shoot();

	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime)override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
};
