// Fill out your copyright notice in the Description page of Project Settings.


#include "STC_Lose_Sight_Target.h"
#include "RobotHunter/AI/AI_Protector/AI_Protector.h"

bool USTC_Lose_Sight_Target::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (!actor)
	{
		LOG_ERROR(CONDITION_LOSE_SIGHT_TARGET_USE_LOG, "actor is null!!");
		return false;
	}
	AAI_Base* _aI = Cast<AAI_Base>(actor);

	return false;
}
