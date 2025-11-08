// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/InteractableActor.h"
#include "TransmitterActor.generated.h"

/**
 * 
 */
UCLASS(HideCategories = ("Interaction"))
class ROBOTHUNTER_API ATransmitterActor : public AInteractableActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Custom|Debug|Transmitter")
	TArray<class UReceiverSystemComponent*> linkedReceiver;

public:
	FORCEINLINE void AddNewLinkedReceiver(class UReceiverSystemComponent* _receiver) { linkedReceiver.Add(_receiver); }
	FORCEINLINE void RemoveLinkedReceiver(class UReceiverSystemComponent* _receiver) { linkedReceiver.Remove(_receiver); }

	ATransmitterActor();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Init() {}

	virtual void Activate();
};
