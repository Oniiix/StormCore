
#include "STTask_Pattern.h"
#include "RobotHunter/AI/Component/Attack/AttackComponent.h"
#include "RobotHunter/AI/Arena/AIsAttackDataAsset.h"
#include <RobotHunter/AI/Arena/Component/ArenaCombat/ArenaCombatComponent.h>
#include "RobotHunter/AI/Arena/Arena.h"

EStateTreeRunStatus USTTask_Pattern::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (owner->IsFighting())
		return EStateTreeRunStatus::Succeeded;
	else
		return EStateTreeRunStatus::Failed;
}
