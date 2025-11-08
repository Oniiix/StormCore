// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ReceiverSystemComponent.h"
#include "SwitchReceiverSystem.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USwitchReceiverSystem : public UReceiverSystemComponent
{
	GENERATED_BODY()
	
protected:
	virtual void ActionOwner() override;
};
