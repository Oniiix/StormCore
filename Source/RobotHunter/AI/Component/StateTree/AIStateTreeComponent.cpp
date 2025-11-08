// Fill out your copyright notice in the Description page of Project Settings.

#include "AIStateTreeComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Controller/CustomAIController.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"

UAIStateTreeComponent::UAIStateTreeComponent()
{
	/// Do NOT set to false => allow stateTree to run
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIStateTreeComponent::InitStateTreeComponent()
{
	/// Check owner availability & bind
	AAI_Base* _owner = Cast<AAI_Base>(GetOwner());
	if (!_owner)
	{
		LOG_ERROR(STATE_TREE_COMPONENT_USE_LOG, "Fail to cast owner in AI_Base on " + _owner->ACTOR_NAME);
		return;
	}
	_owner->OnStun().AddUniqueDynamic(this, &UAIStateTreeComponent::StunAI);
	_owner->OnDie().AddUniqueDynamic(this, &UAIStateTreeComponent::AIDead);

	/// Check CustomController availability & bind
	ACustomAIController* _controller = _owner->GetController<ACustomAIController>();
	if (!_controller)
	{
		LOG_ERROR(STATE_TREE_COMPONENT_USE_LOG, "Fail to get CustomController on " + _owner->ACTOR_NAME);
		return;
	}
	_controller->OnReachedDestination().AddUniqueDynamic(this, &UAIStateTreeComponent::StopMovement);

	/// Check AttackComponent availability & bind 
	UAttackComponent* _attackComponent = _owner->GetAttackComponent();
	if (!_attackComponent)
	{
		LOG_ERROR(STATE_TREE_COMPONENT_USE_LOG, "Fail to get AttackComponent on " + _owner->ACTOR_NAME);
		return;
	}
	_attackComponent->OnEndAttack().AddUniqueDynamic(this, &UAIStateTreeComponent::EndAnimation);

	/// Check TrackingSystem availability & bind
	UTrackingSystemComponent* _trackingSystem = _owner->GetTrackingSystemComponent();
	if (!_trackingSystem)
	{
		LOG_ERROR(STATE_TREE_COMPONENT_USE_LOG, "Failed to get TrackingSystem on " + _owner->ACTOR_NAME);
		return;
	}
	_trackingSystem->OnTargetAcquire().AddUniqueDynamic(this, &UAIStateTreeComponent::TargetAcquire);
	_trackingSystem->OnTargetLost().AddUniqueDynamic(this, &UAIStateTreeComponent::TargetLost);

	/// Check animInstance availability
	UAIBaseAnimInstance* _animInstance = Cast<UAIBaseAnimInstance>(_owner->GetMesh()->AnimScriptInstance);
	if (!_animInstance)
	{
		LOG_ERROR(STATE_TREE_COMPONENT_USE_LOG, "Failed to cast animInstance on UAIBaseAnimInstance on " + _owner->ACTOR_NAME);
		return;
	}
	_animInstance->OnEndAnimation().AddUniqueDynamic(this, &UAIStateTreeComponent::EndAnimation);
}

void UAIStateTreeComponent::TargetAcquire(AMainCharacter* _player)
{
	if (!_player)
	{
		LOG_ERROR(STATE_TREE_COMPONENT_USE_LOG, "Player detected is null !?");
		return;
	}
	SendStateTreeEvent(eventOnTargetAcquire);
	LOG_COLOR(STATE_TREE_COMPONENT_USE_LOG, "Target acquire on StateTree", CYAN);
}

void UAIStateTreeComponent::TargetLost()
{
	SendStateTreeEvent(eventOnTargetLost);
	LOG_COLOR(STATE_TREE_COMPONENT_USE_LOG, "Lost target on StateTree", CYAN);
}

void UAIStateTreeComponent::AIDead(AAI_Base* _AI)
{
	SendStateTreeEvent(eventOnDead);
	LOG_COLOR(STATE_TREE_COMPONENT_USE_LOG, "AI dead on StateTree", CYAN);
}

void UAIStateTreeComponent::StopMovement()
{
	SendStateTreeEvent(eventOnStopMove);
	LOG_COLOR(STATE_TREE_COMPONENT_USE_LOG, "Stop move on StateTree", CYAN);
}

void UAIStateTreeComponent::StunAI(const float _stunTime)
{
	stunTime = _stunTime;
	SendStateTreeEvent(eventOnStun);
	LOG_COLOR(STATE_TREE_COMPONENT_USE_LOG, "Stun AI on StateTree", CYAN);
}

void UAIStateTreeComponent::EndAnimation()
{
	SendStateTreeEvent(eventOnEndAnimation);
	LOG_COLOR(STATE_TREE_COMPONENT_USE_LOG, "End current animation on StateTree", CYAN);
}

void UAIStateTreeComponent::DodgeEnable()
{
	SendStateTreeEvent(eventOnDodge);
}

void UAIStateTreeComponent::StartCamouflage()
{
	SendStateTreeEvent(eventStartCamouflage);
}
