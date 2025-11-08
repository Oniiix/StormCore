#include "STCondition_CanCover.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/Game/GPE/Components/Cover/CoverComponent.h"
#include "RobotHunter/AI/Arena/Component/AIRangePosition/AIRangePositionComponent.h"

bool USTCondition_CanCover::TestCondition(FStateTreeExecutionContext& Context) const
{
	AAI_Hunter* _owner = Cast<AAI_Hunter>(GetOwnerActor(Context));
	if (!_owner)
	{
		LOG_ERROR(true, "Failed to cast owner in AAI_Base");
		return false;
	}
	AArena* _arena = _owner->GetArena();
	if (!_arena)
	{
		LOG_ERROR(true, "Failed to cast owner in _arena");
		return false;
	}
	UAIRangePositionComponent* _range = _arena->GetAIRangePositionComponent();

	const TArray<UCoverComponent*> _listCover = _arena->GetListCover();
	TArray< UCoverComponent*> _listCoverInRange;
	bool _canCover = false;

	for (UCoverComponent* _cover : _listCover)
	{
		const FVector& _coverPosition = _cover->GetActorLocation();
		if (_range->IsAIInOfRange(_coverPosition, player, typeRange))
		{
			_listCoverInRange.Add(_cover);
			_canCover = true;	
		}
	}
	_owner->SetCoverInRange(_listCoverInRange);
	return _canCover;
}
