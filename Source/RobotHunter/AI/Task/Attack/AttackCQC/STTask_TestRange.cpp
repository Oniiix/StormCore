
#include "STTask_TestRange.h"
#include "RobotHunter/AI/Arena/Arena.h"

EStateTreeRunStatus USTTask_TestRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	const FVector& _AILocation = owner->GetActorLocation();
	const AMainCharacter* _AITarget = AITrackingComponent->GetTarget();
	const ERangeAttack& _currentRange = arenaRangeComponent->GetAICurrentRangeOnTarget(_AILocation, _AITarget);

	if (!arenaRangeComponent->IsAIInOfRange(_AILocation, _AITarget))
		return EStateTreeRunStatus::Failed;

	if (_currentRange == rangeToTest)
		return EStateTreeRunStatus::Succeeded;
	else
		return EStateTreeRunStatus::Failed;
}
