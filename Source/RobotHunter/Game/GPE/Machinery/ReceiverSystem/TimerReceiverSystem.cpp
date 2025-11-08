// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerReceiverSystem.h"
#include "../Receiver.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UTimerReceiverSystem::BeginPlay()
{
	Super::BeginPlay();
	if (owner->IsActivate())
		WORLD->GetTimerManager().SetTimer(activationTimer, this, &UTimerReceiverSystem::DisableOwner, activationTime);
}

void UTimerReceiverSystem::ActionOwner()
{
	Super::ActionOwner();
	if (customActivation)
		return;
	if (owner->IsActivate())
		return;
	owner->Action();
	WORLD->GetTimerManager().SetTimer(activationTimer, this, &UTimerReceiverSystem::DisableOwner, activationTime);
}

void UTimerReceiverSystem::DisableOwner()
{
	if (customActivation)
		return;
	owner->Action();
}
