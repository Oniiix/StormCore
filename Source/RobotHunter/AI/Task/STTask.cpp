// Fill out your copyright notice in the Description page of Project Settings.

#include "STTask.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"

EStateTreeRunStatus USTTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	owner = Cast<AAI_Base>(GetOwnerActor(Context));
	if (!owner)
	{
		LOG_ERROR(true, "Failed to cast owner in AAI_Base");
		return EStateTreeRunStatus::Failed;
	}

	arena = owner->GetArena();
	if (!arena)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get arena of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	arenaCombatComponent = arena->GetCombatComponent();
	if (!arenaCombatComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get arenaCombatComponent of arena of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	arenaRangeComponent = arena->GetAIRangePositionComponent();
	if (!arenaRangeComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get arenaRangeComponent of arena of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	AIAttackComponent = owner->GetAttackComponent();
	if (!AIAttackComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get AIAttackComponent of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	AITrackingComponent = owner->GetTrackingSystemComponent();
	if (!AITrackingComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get AITrackingComponent of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	AIAnimInstance = Cast<UAIBaseAnimInstance>(owner->GetMesh()->AnimScriptInstance);
	if (!AIAnimInstance)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get AIAnimInstance of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}
