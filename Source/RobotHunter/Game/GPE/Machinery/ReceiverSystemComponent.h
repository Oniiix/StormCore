// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Game/GPE/Transmitter/TransmitterActor.h"
#include "ReceiverSystemComponent.generated.h"


UCLASS(ABSTRACT)
class ROBOTHUNTER_API UReceiverSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivate);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnActivate onActivate;


	UPROPERTY()
	TObjectPtr<class UNetworkGPEComponent> net = nullptr;

	TArray<ATransmitterActor*> currentActivateTransmitters;

protected:
	UPROPERTY(EditAnywhere, Category="ReceiverSystem|Property")
	bool customActivation = false;
	UPROPERTY(VisibleAnywhere, Category="ReceiverSystem|Debug")
	TObjectPtr<class AReceiver> owner = nullptr;



public:	
	FORCEINLINE FOnActivate& OnActivate() { return onActivate; }
	FORCEINLINE void SetReceiverOwner(AReceiver* _owner) { owner = _owner; }
	FORCEINLINE void SetCustomActivation(const bool _customActivation)
	{
		customActivation = _customActivation;
	}

	UReceiverSystemComponent();

	UFUNCTION() void ActivationFromTransmitter(ATransmitterActor* _transmitter);
protected:
	virtual void BeginPlay() override;

	virtual void ActionOwner();
};
