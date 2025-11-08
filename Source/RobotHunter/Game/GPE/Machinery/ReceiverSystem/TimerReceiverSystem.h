// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ReceiverSystemComponent.h"
#include "TimerReceiverSystem.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTimerReceiverSystem : public UReceiverSystemComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="ReceiverSystem|Timer")
	float activationTime = 5;

	FTimerHandle activationTimer;

public:
	FORCEINLINE float GetActivationTime()const
	{
		return activationTime;
	}

protected:
	virtual void BeginPlay() override;

	virtual void ActionOwner() override;

private:
	UFUNCTION() void DisableOwner();
};
