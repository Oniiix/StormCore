#include "STCondition_TestAttackProjectile.h"
#include "RobotHunter/AI/Arena/AIsAttackDataAsset.h"

bool USTCondition_TestAttackProjectile::TestCondition(FStateTreeExecutionContext& Context) const
{
	AAI_Base* _owner = Cast<AAI_Base>(GetOwnerActor(Context));
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
	UArenaCombatComponent* _combatComponent = _arena->GetCombatComponent();
	if (!_combatComponent)
	{
		LOG_ERROR(true, "Failed to cast owner in _combatComponent");
		return false;
	}
	const TArray<FAIDistanceAttack>& _listDistanceAttack = _combatComponent->GetDistanceAttacksForAI(_owner->GetType(), range);

	return _listDistanceAttack.Num() > 0;
}
