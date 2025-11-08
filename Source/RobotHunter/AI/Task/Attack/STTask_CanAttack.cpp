
#include "STTask_CanAttack.h"

EStateTreeRunStatus USTTask_CanAttack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	/// Check player is down
	AMainCharacter* _target = AITrackingComponent->GetTarget();
	if (!_target)
	{
		LOG_ERROR(true, "Failed to get target of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	if (_target->GetIsDown())
		return EStateTreeRunStatus::Failed;

	/// Check attack height
	const float _heightOwnerToTarget = owner->GetActorLocation().Z - _target->GetActorLocation().Z;
	if (abs(_heightOwnerToTarget) >= maxAttackHeight)
		return EStateTreeRunStatus::Failed;

	/// Check range is in cooldown	
	if (AIAttackComponent->IsInCooldown(ERangeAttack::Short))
		return EStateTreeRunStatus::Failed;

	return EStateTreeRunStatus::Succeeded;
}  
