// Fill out your copyright notice in the Description page of Project Settings.

#include "ProtectorAnimInstance.h"
#include "RobotHunter/AI/AI_Protector/AI_Protector.h"

void UProtectorAnimInstance::InitAnimInstance()
{
	Super::InitAnimInstance();
	AAI_Protector* _AIParalarva = Cast<AAI_Protector>(GetOwningActor());
	if (!_AIParalarva)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to cast in AI_Paralarva");
		return;
	}
}

void UProtectorAnimInstance::LaunchAnimation(const EAIAnimation& _animation)
{
	Super::LaunchAnimation(_animation);

	if (canPlayAnimation)
	{
		switch (_animation)
		{
		case PL_Leap_Attack:
			jumpAttack = true;
			canPlayAnimation = false;
			break;
		case PL_Leap:
			isJumping = true;
			canPlayAnimation = false;
			break;
		}
	}
}
