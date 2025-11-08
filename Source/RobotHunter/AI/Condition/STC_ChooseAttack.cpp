// Fill out your copyright notice in the Description page of Project Settings.


#include "STC_ChooseAttack.h"
#include "StateTreeExecutionContext.h"
#include "RobotHunter/AI/AI_Protector/AI_Protector.h"

bool USTC_ChooseAttack::TestCondition(FStateTreeExecutionContext& Context) const
{
	AAI_Protector* _currentProtector = Cast<AAI_Protector>(Context.GetOwner());
	if (!_currentProtector)
		return false;

	return true;
	//return chooseAttack == _currentProtector->GetChooseAttack();
}
