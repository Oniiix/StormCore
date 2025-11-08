#include "ConditionDataAsset.h"

void UConditionDataAsset::InitCondition(AMainCharacter* _chara)
{
	chara = _chara;
}

void UConditionDataAsset::CancelCondition()
{
	chara = nullptr;
}
