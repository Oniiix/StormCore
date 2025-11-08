// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoutAnimInstance.h"
#include "RobotHunter/AI/AI_Scout/AI_Scout.h"

void UScoutAnimInstance::InitAnimInstance()
{
	Super::InitAnimInstance();

	AAI_Scout* _AIPennant = Cast<AAI_Scout>(owner);
	if (!_AIPennant)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to cast in AI_Pennant");
		return;
	}
}
