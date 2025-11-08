
#include "STTask_InterceptJump.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"

EStateTreeRunStatus USTTask_InterceptJump::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	FTimerHandle _timer;
	Context.GetWorld()->GetTimerManager().SetTimer(_timer, this, &USTTask_InterceptJump::InterceptJump, jumpDelay, true);

	return EStateTreeRunStatus();
}

void USTTask_InterceptJump::InterceptJump()
{
	const int _rand = RANDOM_INT_RANGE(0, 100);
	if (_rand > jumpProbability || owner->GetCharacterMovement()->IsFalling())
		return;

	/// Stop owner movement
	//owner->GetController()->StopMovement();

	/// Calcul direction and launch target
	const FVector& _start = owner->GetActorLocation();
	const FVector& _targetDirection = owner->GetActorForwardVector();
	const FVector& _end = _start + (_targetDirection * 100.f * jumpPower);
	FVector _outLaunchVelocity;

	const bool _isValid = UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), _outLaunchVelocity, _start, _end, 0, pushSnapPower);
	if (_isValid)
	{
		UAIBaseAnimInstance* _animInstance = Cast<UAIBaseAnimInstance>(owner->GetMesh()->AnimScriptInstance);
		if (_animInstance)
			_animInstance->LaunchAnimation(PL_Leap);
		owner->LaunchCharacter(_outLaunchVelocity, true, true);
	}
}
