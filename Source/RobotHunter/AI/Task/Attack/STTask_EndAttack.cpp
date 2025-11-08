
#include "STTask_EndAttack.h"

EStateTreeRunStatus USTTask_EndAttack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	AIAttackComponent->EndAttackCQC();
	AIAnimInstance->EndAnimation();

	return EStateTreeRunStatus::Succeeded;
}
