// Fill out your copyright notice in the Description page of Project Settings.


#include "TorpedoAnimInstance.h"
#include "RobotHunter/AI/AI_Torpedo/AI_Torpedo.h"

void UTorpedoAnimInstance::InitAnimInstance()
{
	Super::InitAnimInstance();

	AAI_Torpedo* AITorpedo = Cast<AAI_Torpedo>(GetOwningActor());
	if (!AITorpedo)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to cast in AI_Torpedo");
		return;
	}
}

