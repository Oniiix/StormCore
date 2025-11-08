
#include "STTask_TestShortRange.h"

EStateTreeRunStatus USTTask_TestShortRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	const FVector& _AILocation = owner->GetActorLocation();
	const AMainCharacter* _AITarget = AITrackingComponent->GetTarget();
	const ERangeAttack& _currentRange = arenaRangeComponent->GetAICurrentRangeOnTarget(_AILocation, _AITarget);

	/// Check IAs not out of range 
	if (!arenaRangeComponent->IsAIInOfRange(_AILocation, _AITarget))
		return EStateTreeRunStatus::Failed;

	if (_currentRange == ERangeAttack::Short)
	{
		///// Check player in sight
		//USightSystemComponent* _sightComponent = owner->GetSightPerceptionComponent();
		//if (!_sightComponent)
		//{
		//	LOG_ERROR(true, "Failed to get sight component of " + owner->ACTOR_NAME);
		//	return EStateTreeRunStatus::Failed;
		//}
		//FHitResult _result;
		//const FVector& _start = owner->GetActorLocation();
		//const FVector& _end = _AITarget->GetActorLocation();
		//const bool _hasHit = UKismetSystemLibrary::LineTraceSingleForObjects(this, _start, _end, _sightComponent->GetLayersDetection(), false, { owner },
		//	EDrawDebugTrace::None, _result, true);
		//if (_hasHit)
		//	return EStateTreeRunStatus::Succeeded;
		//else
		//	return EStateTreeRunStatus::Failed;
		return EStateTreeRunStatus::Succeeded;
	}
	else
		return EStateTreeRunStatus::Failed;
}
