#include "STTask_Dodge.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RobotHunter/AI/Animation/HunterAnimInstance.h"

EStateTreeRunStatus USTTask_Dodge::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	float _distance = 0;

	const FVector& _actorLocation = owner->GetActorLocation();
	const FVector& _actorRightDirection = owner->GetActorRightVector();
	const FVector& bounds = owner->GetRootComponent()->GetLocalBounds().BoxExtent;

	FHitResult _result;
	int _index = 0;

	for (int i = -1; i <= 1; i += 2)
	{
		const FVector& _targetPosition = _actorLocation + _actorRightDirection * lengthDash * i;
		const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _actorLocation, _targetPosition, layerDodge, true, TArray<AActor*>(), EDrawDebugTrace::None, _result, true);
		if (!_hit)
		{
			_index = i;
			targetPosition = _targetPosition;
			break;
		}
		const float _dist = FVector::Dist(_actorLocation, _result.ImpactPoint);
		if(_distance < _dist)
		{
			_index = i;
			_distance = _dist;
			targetPosition = _result.ImpactPoint - _actorRightDirection * bounds.Y * i;
		}
	}
	const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), targetPosition, targetPosition - owner->GetActorUpVector() * 500, layerDodge, true, TArray<AActor*>(), EDrawDebugTrace::None, _result, true);
	if (_hit)
		targetPosition = GetNavigableLocation(_result.ImpactPoint);
	UHunterAnimInstance* _anim = Cast< UHunterAnimInstance>(AIAnimInstance);
	_anim->Dodge(_index);
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_Dodge::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	const FVector& _actorLocation = owner->GetActorLocation();

	const FVector& _targetPosition = FVector(targetPosition.X, targetPosition.Y, _actorLocation.Z);
	FVector _newLocation = FMath::VInterpConstantTo(_actorLocation, _targetPosition, DeltaTime, speedDodge);

	owner->SetActorLocation(_newLocation);

	return _newLocation == _targetPosition ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
}
