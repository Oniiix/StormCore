
#include "STTask_CheckAI.h"
#include "RobotHunter/AI/Arena/Arena.h"

EStateTreeRunStatus USTTask_CheckAI::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	const int _distanceArenaToAI = FVector::Dist(arena->GetActorLocation(), owner->GetActorLocation());
	if (_distanceArenaToAI >= arena->GetTrackingRange())
	{
		owner->PlayDeath();
		return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}
