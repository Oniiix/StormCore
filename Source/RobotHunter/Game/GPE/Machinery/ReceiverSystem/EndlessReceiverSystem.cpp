// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessReceiverSystem.h"
#include "../Receiver.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UEndlessReceiverSystem::BeginPlay()
{
	Super::BeginPlay();

	if (owner->IsActivate())
		StopSystem();
}

void UEndlessReceiverSystem::ActionOwner()
{
	Super::ActionOwner();
	if (customActivation)
		return;

	owner->Action();
	FTimerHandle _stopTimer;
	WORLD->GetTimerManager().SetTimer(_stopTimer, this, &UEndlessReceiverSystem::StopSystem, 0.1f);
}

void UEndlessReceiverSystem::StopSystem()
{
	TArray<ATransmitterActor*> _transmittersList = owner->GetTransmitters();
	for (ATransmitterActor* _transmitter : _transmittersList)
	{
		if (_transmitter)
			_transmitter->RemoveLinkedReceiver(this);
	}
}
