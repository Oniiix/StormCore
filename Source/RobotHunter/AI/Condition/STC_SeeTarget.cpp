// Fill out your copyright notice in the Description page of Project Settings.


#include "STC_SeeTarget.h"
#include "RobotHunter/AI/AI_Protector/AI_Protector.h"

bool USTC_SeeTarget::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (!actor)
	{
		LOG_ERROR(CONDITION_SEE_TARGET_USE_LOG, "actor is null !!");
		return false;
	}
	AAI_Base* _aI = Cast<AAI_Base>(actor);

	return false;
}
