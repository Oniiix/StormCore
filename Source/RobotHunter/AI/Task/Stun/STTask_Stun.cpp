// Objectif 3D Copyright

#include "STTask_Stun.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"

EStateTreeRunStatus USTTask_Stun::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	/// Stop owner movement
	owner->GetController()->StopMovement();
	owner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	/// Launch animation
	AIAnimInstance->StartStun(stunTime);
	/// Stop Attack if started
	AIAttackComponent->EndAttackCQC();

	FTimerHandle _timerStun;
	GetWorld()->GetTimerManager().SetTimer(_timerStun, this, &USTTask_Stun::EndTimerStun, stunTime);
		
	return EStateTreeRunStatus::Running; 
}

EStateTreeRunStatus USTTask_Stun::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	if (endTimerStun)
		return EStateTreeRunStatus::Succeeded;

	return EStateTreeRunStatus::Running;
}

void USTTask_Stun::EndTimerStun()
{
	endTimerStun = true;
}
