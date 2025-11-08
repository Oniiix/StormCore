// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchReceiverSystem.h"
#include "../Receiver.h"

void USwitchReceiverSystem::ActionOwner()
{
	Super::ActionOwner();
	if (customActivation)
		return;
	owner->Action();
}
