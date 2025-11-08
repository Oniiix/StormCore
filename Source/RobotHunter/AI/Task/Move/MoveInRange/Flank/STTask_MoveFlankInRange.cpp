#include "STTask_MoveFlankInRange.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/Component/AIRangePosition/AIRangePositionComponent.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"

EStateTreeRunStatus USTTask_MoveFlankInRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	AAI_Hunter* _hunter = Cast< AAI_Hunter>(owner);
	if (!_hunter)
		return EStateTreeRunStatus::Failed;

	UAIRangePositionComponent* _range = owner->GetArena()->GetAIRangePositionComponent();

	const FVector& _actorLocation = owner->GetActorLocation();
	const FVector& _playerLocation = player->GetActorLocation();

	const FVector& _directionFinalPoint = (_playerLocation - _actorLocation).GetSafeNormal();

	const float _distance = FVector::Dist(_actorLocation, _playerLocation);
	const FVector& _finalPoint = _playerLocation + _directionFinalPoint * _distance;
	const FVector& _interPoint = _playerLocation + owner->GetActorRightVector() * _distance;
	
	const FPointRangeArena& _flankPoint = _range->GetPointToLocation(_finalPoint, range, player, true, offSetShoot);
	if(_flankPoint.index == -1)
	{
		movementComponent->Move(owner->GetActorLocation(), acceptanceRadius);
		return EStateTreeRunStatus::Running;
	}
	_hunter->SetFlankPoints(_flankPoint);

	const FPointRangeArena& _aroundPoint = _range->GetPointToLocation(_interPoint, range, player);
	movementComponent->SetTargetPoint(_aroundPoint);

	movementComponent->Move(_aroundPoint.position, acceptanceRadius);

	return EStateTreeRunStatus::Running;
}
