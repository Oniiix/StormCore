


#include "STTask_Jump.h"

EStateTreeRunStatus USTTask_Jump::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	maxHeightPosition = owner->GetActorLocation() + owner->GetActorUpVector() * jumpHeight;

	UCharacterMovementComponent* _characterMovement = owner->GetCharacterMovement();
	_characterMovement->MovementMode = EMovementMode::MOVE_Flying;
	_characterMovement->GravityScale = 0;

	owner->GetMesh()->SetEnableGravity(false);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_Jump::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	const FVector& _newLocation = FMath::VInterpConstantTo(owner->GetActorLocation(), maxHeightPosition, GetWorld()->DeltaTimeSeconds, speedJump);
	owner->SetActorLocation(_newLocation);
	
	if (_newLocation.Z == maxHeightPosition.Z)
		return EStateTreeRunStatus::Succeeded;

	return EStateTreeRunStatus::Running;
}
