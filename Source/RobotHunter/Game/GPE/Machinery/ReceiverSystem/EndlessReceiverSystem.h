// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ReceiverSystemComponent.h"
#include "EndlessReceiverSystem.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UEndlessReceiverSystem : public UReceiverSystemComponent
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

	virtual void ActionOwner() override;
	void StopSystem();
};
