// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "STTask_MoveInRange.generated.h"

#define TASK_MOVE_IN_RANGE_USE_LOG false

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API USTTask_MoveInRange : public USTTask_Movement
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
    float offSetShoot = 200;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
    bool canMoveRange = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
    TEnumAsByte<ERangeAttack> typeRange = ERangeAttack::Long;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
    TObjectPtr<AMainCharacter> player = nullptr;
   
protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
