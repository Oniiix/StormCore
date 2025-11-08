// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "RobotHunter/Utils/ENUM_Projectile.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "STTask_Shoot.generated.h"

#define TASK_SHOOT_USE_LOG false

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API USTTask_Shoot : public USTTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<AMainCharacter> target = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EProjectileRange> projectileRange = EProjectileRange::BOTH_RANGE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<ERangeAttack> range;

	TObjectPtr<UAIMovementComponent> movementComponent = nullptr;
	
protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime)override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
};
