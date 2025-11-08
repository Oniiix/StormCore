#include "STTask_SelectRange.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/Utils/ENUM_AI.h"

EStateTreeRunStatus USTTask_SelectRange::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	AArena* _arena = owner->GetArena();
	if (!_arena)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get arena of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	UArenaCombatComponent* _arenaCombatComponent = _arena->GetCombatComponent();
	if (!_arenaCombatComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get arenaCombatComponent of arena of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	UAIRangePositionComponent* _arenaRangeComponent = _arena->GetAIRangePositionComponent();
	if (!_arenaRangeComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get arenaRangeComponent of arena of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	UAttackComponent* _AIAttackComponent = owner->GetAttackComponent();
	if (!_AIAttackComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get AIAttackComponent of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}
	UTrackingSystemComponent* _AITrackingComponent = owner->GetTrackingSystemComponent();
	if (!_AITrackingComponent)
	{
		LOG_ERROR(TASK_PATTERN_USE_LOG, "Failed to get AITrackingComponent of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}


	TSortedMap<TEnumAsByte<ERangeAttack>, FAIRangeAttack> _mapRange = _AIAttackComponent->GetMapRange();
	const int _mapRangeCount = _mapRange.Num();
	const FVector& _AILocation = owner->GetActorLocation();
	const AMainCharacter* _AITarget = _AITrackingComponent->GetTarget();
	if (!_AITarget)
		return EStateTreeRunStatus::Failed;
	const FVector& _AITargetLocation = _AITarget->GetActorLocation();
	ERangeAttack _currentRange = _arenaRangeComponent->GetAICurrentRangeOnTarget(_AILocation, _AITarget);

	TArray<TEnumAsByte<ERangeAttack>> _listRange;
	_mapRange.GenerateKeyArray(_listRange);
	TEnumAsByte<ERangeAttack> _firstRange = _listRange[0];
	TEnumAsByte<ERangeAttack> _lastRange = _listRange.Last();

	rangeToMove = _currentRange;
	/// If out of range => Move to max range
	if (!_arenaRangeComponent->IsAIInOfRange(_AILocation, _AITarget))
	{
		rangeToMove = _lastRange;
	}
	/// If wrong range => Move to closest range
	else if (!_mapRange.Contains(_currentRange))
	{
		/// Get closest range from bottom/top
		rangeToMove = _currentRange < _firstRange ? _firstRange : _lastRange;
	}
	/// If currentRange in cooldown
	else if (_mapRange[_currentRange].isInCooldown)
	{
		_listRange.Remove(_currentRange);
		const int _listRangeCount = _listRange.Num();
		if (_listRangeCount != 0) /// No other range => Move in range
		{
			const int _startIndex = _currentRange < _listRange[0] ? 0 : _listRangeCount - 1; /// Focus outside
			const int _step = _startIndex == 0 ? 1 : -1;
			/// Move to available range
			for (int i = _startIndex; _startIndex == 0 ? i < _listRangeCount : i >= 0; i += _step)
			{
				if (_mapRange[_listRange[i]].isInCooldown)
					continue;
				rangeToMove = _listRange[i];
			}
		}
	}

	owner->SetRange(rangeToMove);
	return EStateTreeRunStatus::Running;
}