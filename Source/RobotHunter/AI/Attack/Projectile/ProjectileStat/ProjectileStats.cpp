// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileStats.h"
#include "RobotHunter/Utils/DebugUtils.h"

#if WITH_EDITOR
void UProjectileStats::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UProjectileStats, isDestroyOnPlayer) && isDestroyOnPlayer)
		canDuringHit = false;
}
#endif