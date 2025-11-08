#include "STCondition_CanFlank.h"

bool USTCondition_CanFlank::TestCondition(FStateTreeExecutionContext& Context) const
{
	return percentageRand > FMath::FRandRange(0.f, 100.f);
}
