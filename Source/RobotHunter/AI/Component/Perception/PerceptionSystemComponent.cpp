// Fill out your copyright notice in the Description page of Project Settings.

#include "PerceptionSystemComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/CustomPlayerController.h"

UPerceptionSystemComponent::UPerceptionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	owner = nullptr;
	isInitializationGood = false;
}

void UPerceptionSystemComponent::InitPerception()
{
	/// Check Owner availability
	owner = Cast<AAI_Base>(GetOwner());
	if (!owner)
	{
		LOG_ERROR(PERCEPTION_SYSTEM_COMPONENT_USE_LOG, "Failed to Cast owner in AI_Base");
		return;
	}
	/// Check layersDetection availability
	if (layersDetection.IsEmpty())
	{
		LOG_WARNING(PERCEPTION_SYSTEM_COMPONENT_USE_LOG, "No layers detection on " + OWNER_NAME);
		return;
	}

	/// Delay Tick for optimisation
	SetComponentTickInterval(DELTA_TICK);
	SetComponentTickEnabled(!disablePerception);
}

void UPerceptionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isInitializationGood)
		return;

	DrawDebug();
	PerceptionBehaviour();
}

void UPerceptionSystemComponent::PerceptionBehaviour()
{
	/// nothing
}

void UPerceptionSystemComponent::Detected(AActor* _actorDetected)
{
	AMainCharacter* _target = Cast<AMainCharacter>(_actorDetected);
	if (!_target)
	{
		LOG_ERROR(PERCEPTION_SYSTEM_COMPONENT_USE_LOG, "Failed to cast actor detected in AMainCharacter !!");
		return;
	}
	LOG_COLOR(PERCEPTION_SYSTEM_COMPONENT_USE_LOG, "Detected", TURQUOISE);

	/// Check customPlayerController availability
	ACustomPlayerController* _pc = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!_pc)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "Failed to get ACustomPlayerController in " + OWNER_NAME);
		return;
	}
	/// Update on server
	OWNER_AUTHORITY ? onDetected.Broadcast(_target) : _pc->NetworkAI()->ServerRPC_PlayerDetected(this, _target);
}

void UPerceptionSystemComponent::EnablePerception(const bool _enable)
{
	const bool _tickable = !disablePerception && _enable;
	SetComponentTickEnabled(_tickable);
}

bool UPerceptionSystemComponent::DrawDebug()
{
	if (!isInitializationGood || !drawDebug || !owner->GetDrawDebug() || !owner->IsEnable())
		return false;

	return true;
}