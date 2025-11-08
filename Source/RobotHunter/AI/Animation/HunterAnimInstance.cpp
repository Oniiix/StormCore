// Fill out your copyright notice in the Description page of Project Settings.

#include "HunterAnimInstance.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"

void UHunterAnimInstance::InitAnimInstance()
{
	Super::InitAnimInstance();

	AAI_Hunter* _AITrapper = Cast<AAI_Hunter>(GetOwningActor());
	if (!_AITrapper)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to cast owner in AI_Trapper");
		return;
	}
}

void UHunterAnimInstance::LaunchAnimation(const EAIAnimation& _animation)
{
	Super::LaunchAnimation(_animation);
	if (canPlayAnimation)
	{
		switch (_animation)
		{
		case TRP_CQC:
			jumpAttack = true;
			canPlayAnimation = false;
			break;
		case TRP_Acid_Shot:
			onAcidShot = true;
			canPlayAnimation = false;
			break;
		case TRP_Direct_Shot:
			onDirectShot = true;
			canPlayAnimation = false;
			break;
		}
	}
}

void UHunterAnimInstance::EndAnimation()
{
	Super::EndAnimation();
	onAcidShot = false;
	onDirectShot = false;
	onDodge = false;
}

void UHunterAnimInstance::Dodge(const int _dodgeRight)
{
	onDodge = true;
	canPlayAnimation = false;
	dodgeDirection = _dodgeRight;
}
