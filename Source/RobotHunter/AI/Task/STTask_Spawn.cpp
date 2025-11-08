
#include "STTask_Spawn.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"

EStateTreeRunStatus USTTask_Spawn::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (owner->IsSpawned())
	{
		UAIBaseAnimInstance* _animInstance = Cast<UAIBaseAnimInstance>(owner->GetMesh()->AnimScriptInstance);
		_animInstance->LaunchAnimation(EAIAnimation::Spawn);
		return EStateTreeRunStatus::Running;
	}
	else
		return EStateTreeRunStatus::Succeeded;

}
