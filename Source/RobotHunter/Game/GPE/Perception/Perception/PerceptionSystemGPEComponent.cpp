// Fill out your copyright notice in the Description page of Project Settings.


#include "PerceptionSystemGPEComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"



UPerceptionSystemGPEComponent::UPerceptionSystemGPEComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
}




void UPerceptionSystemGPEComponent::BeginPlay()
{
	Super::BeginPlay();
	if (disablePerception)
		DesactivatePerception();
	
	
	
}
void UPerceptionSystemGPEComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PerceptionBehaviour();
	DrawDebug();
}


void UPerceptionSystemGPEComponent::Detected(AActor* _actorDetected)
{
	if (!_actorDetected || target)
		return;

	hasTarget = true;
	target = _actorDetected;
	
	LOG_COLOR(0, "Detected", TURQUOISE);

	
	INVOKE(onDetected, target);
}

void UPerceptionSystemGPEComponent::ClearTarget()
{
	hasTarget = false;
	if (target)
		onLost.Broadcast();


	target = nullptr;
}

void UPerceptionSystemGPEComponent::ActivatePerception()
{
	disablePerception = false;
	SetComponentTickEnabled(true);
}
void UPerceptionSystemGPEComponent::DesactivatePerception()
{
	disablePerception = true;
	SetComponentTickEnabled(false);
}