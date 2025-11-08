#include "STTask_MoveCoverInRange.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"

EStateTreeRunStatus USTTask_MoveCoverInRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	AArena* _arena = owner->GetArena();
	if (!_arena)
	{
		LOG_WARNING(true, "Failed to get arena on " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}

	if (!player)
		return EStateTreeRunStatus::Failed;

	AAI_Hunter* _hunter = Cast<AAI_Hunter>(owner);
	if (!_hunter)
		return EStateTreeRunStatus::Failed;
	hunter = _hunter;
	acceptanceRadius = arenaRangeComponent->GetAcceptanceRadiusAI();

	const FVector& _aiLocation = owner->GetActorLocation();

	FVector _newLocation;

	UCoverComponent* _cover = GetCover(listCover, _aiLocation);

	hunter->SetCurrentCover(_cover);

	FPointRangeArena _point = _cover->GetPoint(player->GetActorLocation(), _aiLocation);

	_newLocation = GetNavigableLocationInArena(_point.position);

	movementComponent->Move(_newLocation, acceptanceRadius);
	movementComponent->SetCanMoveInRange(false);

	return EStateTreeRunStatus::Running;
}

void USTTask_MoveCoverInRange::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	if (forceFlank)
		hunter->SetCurrentCover(nullptr);
}

UCoverComponent* USTTask_MoveCoverInRange::GetCover(const TArray<UCoverComponent*> _listCover, const FVector& _actorLocation)
{
	UCoverComponent* _targetCover = nullptr;
	float _dist = TNumericLimits<int>::Max();
	TArray< UCoverComponent*> _allCovers = _listCover;

	UCoverComponent* _currentCover = hunter->GetCurrentCover();

	if(_listCover.Contains(_currentCover))
	{
		if (_listCover.Num() == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("forceFlank"));
			forceFlank = true;
		}
		else
			_allCovers.Remove(_currentCover);
	}

	for (UCoverComponent* _cover : _allCovers)
	{
		const float _distanceActor = FVector::Dist(_actorLocation, _cover->GetActorLocation());
		if (_dist > _distanceActor)
		{
			_targetCover = _cover;
			_dist = _distanceActor;
		}
	}

	return _targetCover;
}
