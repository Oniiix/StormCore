// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveProjectileStat.h"
#include "RobotHunter/Utils/DebugUtils.h"

UCurveProjectileStat::UCurveProjectileStat()
{
	useCurveType = false;
	definitionCurve = 30;
}

#if WITH_EDITOR
void UCurveProjectileStat::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UCurveProjectileStat, definitionCurve) && definitionCurve < 30)
		definitionCurve = 30;
}
#endif
