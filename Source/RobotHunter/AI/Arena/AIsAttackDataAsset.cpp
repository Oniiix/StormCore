
#include "AIsAttackDataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"

bool UAIsAttackDataAsset::CheckAvailability()
{
	const int _listShortRangeAttackCount = listShortRangeAttack.Num();
	if (_listShortRangeAttackCount == 0)
	{
		//LOG_WARNING(true, FString::Format(TEXT("List CQC Attack in AIsAttack DataAsset in ArenaCombatComponent of {0} is empty"), { OWNER_NAME }));
		return false;
	}
	for (int i = 0; i < _listShortRangeAttackCount; i++)
	{
		if (!listShortRangeAttack[i].CQCAttack)
		{
			//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("CQC Attack ref at index {0} in AIsAttack DataAsset in ArenaCombatComponent of {1} is null"), { i, OWNER_NAME }));
			return false;
		}
		TArray<TEnumAsByte<EAIType>> _listAIsAllowed = listShortRangeAttack[i].listAIsAllowed;
		const int _listAIsAllowedCount = _listAIsAllowed.Num();
		if (_listAIsAllowedCount == 0)
		{
			//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("List AIs Allowed index {0} in AIsAttack DataAsset in ArenaCombatComponent of {1} is empty"), { i, OWNER_NAME }));
			return false;
		}
		for (int j = 0; j < _listAIsAllowedCount; j++)
		{
			if (_listAIsAllowed[j] == EAIType::Base)
			{
				//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("AI allowed type 'Base' at index {0} in AIsAttackDataAsset in ArenaCombatComponent of {1} is not allow"), { j, OWNER_NAME }));
				return false;
			}
		}
	}

	const int _listMidRangeAttackCount = listMidRangeAttack.Num();
	if (_listMidRangeAttackCount == 0)
	{
		//LOG_WARNING(true, FString::Format(TEXT("List CQC Attack in AIsAttack DataAsset in ArenaCombatComponent of {0} is empty"), { OWNER_NAME }));
		return false;
	}
	for (int i = 0; i < _listMidRangeAttackCount; i++)
	{
		if (!listMidRangeAttack[i].projectileRef)
		{
			//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("CQC Attack ref at index {0} in AIsAttack DataAsset in ArenaCombatComponent of {1} is null"), { i, OWNER_NAME }));
			return false;
		}
		TArray<TEnumAsByte<EAIType>> _listAIsAllowed = listMidRangeAttack[i].listAIsAllowed;
		const int _listAIsAllowedCount = _listAIsAllowed.Num();
		if (_listAIsAllowedCount == 0)
		{
			//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("List AIs Allowed index {0} in AIsAttack DataAsset in ArenaCombatComponent of {1} is empty"), { i, OWNER_NAME }));
			return false;
		}
		for (int j = 0; j < _listAIsAllowedCount; j++)
		{
			if (_listAIsAllowed[j] == EAIType::Base)
			{
				//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("AI allowed type 'Base' at index {0} in AIsAttackDataAsset in ArenaCombatComponent of {1} is not allow"), { j, OWNER_NAME }));
				return false;
			}
		}
	}

	const int _listLongRangeAttackCount = listLongRangeAttack.Num();
	if (_listLongRangeAttackCount == 0)
	{
		//LOG_WARNING(true, FString::Format(TEXT("List CQC Attack in AIsAttack DataAsset in ArenaCombatComponent of {0} is empty"), { OWNER_NAME }));
		return false;
	}
	for (int i = 0; i < _listLongRangeAttackCount; i++)
	{
		if (!listLongRangeAttack[i].projectileRef)
		{
			//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("CQC Attack ref at index {0} in AIsAttack DataAsset in ArenaCombatComponent of {1} is null"), { i, OWNER_NAME }));
			return false;
		}
		TArray<TEnumAsByte<EAIType>> _listAIsAllowed = listLongRangeAttack[i].listAIsAllowed;
		const int _listAIsAllowedCount = _listAIsAllowed.Num();
		if (_listAIsAllowedCount == 0)
		{
			//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("List AIs Allowed index {0} in AIsAttack DataAsset in ArenaCombatComponent of {1} is empty"), { i, OWNER_NAME }));
			return false;
		}
		for (int j = 0; j < _listAIsAllowedCount; j++)
		{
			if (_listAIsAllowed[j] == EAIType::Base)
			{
				//LOG_ERROR(COMBAT_COMPONENT_USE_LOG, FString::Format(TEXT("AI allowed type 'Base' at index {0} in AIsAttackDataAsset in ArenaCombatComponent of {1} is not allow"), { j, OWNER_NAME }));
				return false;
			}
		}
	}

	return true;
}

void UAIsAttackDataAsset::GetToken(const ERangeAttack& _range, const int _index)
{
	switch (_range)
	{
	case ERangeAttack::Short:
		//LOG_COLOR(true, "Get Token on Short range at index " + FString::FromInt(_index), WHITE);
		if (listShortRangeAttack[_index].token >= 0)
			listShortRangeAttack[_index].token--;
		break;
	case ERangeAttack::Mid:
		//LOG_COLOR(true, "Get Token on Mid range at index " + FString::FromInt(_index), WHITE);
		if (listMidRangeAttack[_index].token >= 0)
			listMidRangeAttack[_index].token--;
		break;
	case ERangeAttack::Long:
		//LOG_COLOR(true, "Get Token on long range at index " + FString::FromInt(_index), WHITE);
		if (listLongRangeAttack[_index].token >= 0)
			listLongRangeAttack[_index].token--;
		break;
	}
}

void UAIsAttackDataAsset::GenerateToken(const ERangeAttack& _range, const int _index)
{
	switch (_range)
	{
	case ERangeAttack::Short:
		//LOG_COLOR(true, "Generate Token on Short range at index " + FString::FromInt(_index), BLACK);
		CLAMP(listShortRangeAttack[_index].token++, 0, listShortRangeAttack[_index].maxToken);
		break;
	case ERangeAttack::Mid:
		//LOG_COLOR(true, "Generate Token on Mid range at index " + FString::FromInt(_index), BLACK);
		CLAMP(listMidRangeAttack[_index].token++, 0, listMidRangeAttack[_index].maxToken);
		break;
	case ERangeAttack::Long:
		//LOG_COLOR(true, "Generate Token on Long range at index " + FString::FromInt(_index), BLACK);
		CLAMP(listLongRangeAttack[_index].token++, 0, listLongRangeAttack[_index].maxToken);
		break;
	}
}
