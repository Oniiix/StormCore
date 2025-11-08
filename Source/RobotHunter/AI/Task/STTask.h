// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeExecutionContext.h"
#include "RobotHunter/AI/AI_Base.h"
#include "STTask.generated.h"

class AArena;
class UArenaCombatComponent;
class UAIRangePositionComponent;
class UAttackComponent;
class UTrackingSystemComponent;
class UAIBaseAnimInstance;

UCLASS(HideDropdown)
class ROBOTHUNTER_API USTTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	TObjectPtr<AAI_Base> owner = nullptr;
	TObjectPtr<AArena> arena = nullptr;
	TObjectPtr<UArenaCombatComponent> arenaCombatComponent = nullptr;
	TObjectPtr<UAIRangePositionComponent> arenaRangeComponent = nullptr;
	TObjectPtr<UAttackComponent> AIAttackComponent = nullptr;
	TObjectPtr<UTrackingSystemComponent> AITrackingComponent = nullptr;
	TObjectPtr<UAIBaseAnimInstance> AIAnimInstance = nullptr;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
