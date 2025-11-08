// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_Debug.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Component/Widget/AIDebugInfoWidgetComponent.h"

EStateTreeRunStatus USTTask_Debug::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	UAIDebugInfoWidgetComponent* _debugInfoComponent = owner->GetDebugInfoComponent();
	if (_debugInfoComponent)
		_debugInfoComponent->SetStateInfo(text, color);

	return EStateTreeRunStatus::Running;
}
