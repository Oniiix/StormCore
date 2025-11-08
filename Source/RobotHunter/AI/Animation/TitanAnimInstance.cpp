// Fill out your copyright notice in the Description page of Project Settings.

#include "TitanAnimInstance.h"
#include "RobotHunter/AI/AI_Giant/AI_Giant.h"

void UTitanAnimInstance::InitAnimInstance()
{
	Super::InitAnimInstance();

	AAI_Giant* _AITitan = Cast<AAI_Giant>(GetOwningActor());
	if (!_AITitan)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to cast in AI_Titan");
		return;
	}

	_AITitan->OnEnableShield().AddUniqueDynamic(this, &UTitanAnimInstance::EnableShield);
	_AITitan->OnDisableShield().AddUniqueDynamic(this, &UTitanAnimInstance::DisableShield);
}

void UTitanAnimInstance::LaunchAnimation(const EAIAnimation& _animation)
{
	Super::LaunchAnimation(_animation);
	if (canPlayAnimation)
	{
		switch (_animation)
		{
		case TT_CQC:
			jumpAttack = true;
			canPlayAnimation = false;
			break;
		}
	}
}

void UTitanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/// Lerp shield anim
	shieldBlendAlpha = UKismetMathLibrary::FInterpTo_Constant(shieldBlendAlpha, isInShield, DeltaSeconds, speedShieldBlendAlpha);
}

void UTitanAnimInstance::EnableShield()
{
	isInShield = true;
}

void UTitanAnimInstance::DisableShield()
{
	isInShield = false;
}
