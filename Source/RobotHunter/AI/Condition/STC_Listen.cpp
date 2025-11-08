// Fill out your copyright notice in the Description page of Project Settings.


#include "STC_Listen.h"

bool USTC_Listen::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (!actor)
	{
		LOG_ERROR(CONDITION_LISTEN_USE_LOG, "actor is null!!");
		return false;
	}
	AAI_Base* _AI = Cast<AAI_Base>(actor);

	//if (!_AI || !_AI->GetListenPerceptionComponent())
	//{
	//	LOG_ERROR(CONDITION_LISTEN_USE_LOG, "Failed to cast actor in AI_Base OR Failed to get ListenPerceptionComponent");
	//	return false;
	//}

	return false;

}
