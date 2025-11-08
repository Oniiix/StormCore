// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_DisableCamouflage.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "RobotHunter/AI/Component/Camouflage/CamouflageComponent.h"
#include "RobotHunter/Network/Component/NetworkAIComponent.h"

EStateTreeRunStatus USTTask_DisableCamouflage::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	AAI_Hunter* _hunter = Cast<AAI_Hunter>(owner);
	if (!_hunter)
	{
		LOG_ERROR(TASK_DISABLE_CAMOUFLAGE_USE_LOG, "Failed to cast owner in AI_Hunter");
		return EStateTreeRunStatus::Failed;
	}
	UCamouflageComponent* _camouflage = _hunter->GetCamouflageComponent();
	if (!_camouflage)
	{
		LOG_ERROR(TASK_DISABLE_CAMOUFLAGE_USE_LOG, "Failed to get UCamouflageComponent");
		return EStateTreeRunStatus::Failed;
	}
	UNetworkAIComponent* _net = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkAIComponent>();
	_net->MulticastRPC_DisableTransformMode(_camouflage);
	return EStateTreeRunStatus::Running;
}
