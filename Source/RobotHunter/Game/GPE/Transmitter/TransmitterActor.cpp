// Fill out your copyright notice in the Description page of Project Settings.


#include "TransmitterActor.h"
#include "RobotHunter/Game/GPE/Machinery/ReceiverSystemComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"

ATransmitterActor::ATransmitterActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	canBeDetected = false;

	//interactionName = TEXTSTR("Activate");
}

void ATransmitterActor::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ATransmitterActor::Activate()
{
	for (UReceiverSystemComponent* _receiver : linkedReceiver)
		_receiver->ActivationFromTransmitter(this);
}
