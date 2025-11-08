#include "STTask_DisableJump.h"
#include "RobotHunter/AI/AI_Scout/AI_Scout.h"

EStateTreeRunStatus USTTask_DisableJump::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	UCharacterMovementComponent* _characterMovement = owner->GetCharacterMovement();
	_characterMovement->MovementMode = EMovementMode::MOVE_Walking;
	_characterMovement->GravityScale = 1;
	owner->GetMesh()->SetEnableGravity(true);
	AAI_Scout* _scout = Cast<AAI_Scout>(owner);
	if (_scout)
		_scout->SetMarkCanJump(true);

	return EStateTreeRunStatus::Running;
}
