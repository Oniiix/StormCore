// Fill out your copyright notice in the Description page of Project Settings.


#include "ReceiverSystemComponent.h"
#include "Receiver.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"

UReceiverSystemComponent::UReceiverSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UReceiverSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = Cast<AReceiver>(GetOwner());
	net = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>();
	TArray<ATransmitterActor*> _transmittersList = owner->GetTransmitters();
	for (ATransmitterActor* _transmitter : _transmittersList)
	{
		if (_transmitter)
			_transmitter->AddNewLinkedReceiver(this);
	}
}

void UReceiverSystemComponent::ActionOwner()
{
	if (GetOwner()->HasAuthority())
	{
		if (customActivation)
			onActivate.Broadcast();
	}
	else
		net->ServerRpc_Action(owner);
}


void UReceiverSystemComponent::ActivationFromTransmitter(ATransmitterActor* _transmitter)
{
	currentActivateTransmitters.AddUnique(_transmitter);
	if (currentActivateTransmitters.Num() == owner->GetNumTransForActivation())
	{
		currentActivateTransmitters.Empty();
		ActionOwner();
	}
}