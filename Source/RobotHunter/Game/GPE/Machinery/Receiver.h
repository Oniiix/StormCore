// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/Utils/ENUM_Receiver.h"
#include "ReceiverSystemComponent.h"
#include "Receiver.generated.h"


UCLASS(ABSTRACT)
class ROBOTHUNTER_API AReceiver : public ACustomActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAction, bool, _isActive);

protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnAction onAction;


	UPROPERTY(EditInstanceOnly, Category = "Custom|Receiver")
	EReceiverType type = EReceiverType::Receiver_None;
	UPROPERTY(EditAnywhere, Category = "Custom|Receiver")
	TArray<ATransmitterActor*> transmitters;
	UPROPERTY(EditAnywhere, Category = "Custom|Receiver", meta =(UMin = 1, ClampMin = 1))
	int numTransmitterForActivation = 1;
	UPROPERTY(EditAnywhere, Category = "Custom|Receiver")
	bool startActivate = false;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateIsActive, VisibleAnywhere, Category = "Custom|Receiver")
	bool isReceiverActive = false;

	bool hasFinishAction = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UReceiverSystemComponent> receiverSystem = nullptr;



public:
	FORCEINLINE FOnAction& OnAction() { return onAction; }

	FORCEINLINE bool IsActivate() const { return isReceiverActive; }
	FORCEINLINE TArray<ATransmitterActor*> GetTransmitters() const { return transmitters; }
	FORCEINLINE int GetNumTransForActivation() const { return numTransmitterForActivation; }

	AReceiver();

public:
	virtual void Action();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void DrawDebug();
	virtual void InitReceiver() {}

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UFUNCTION() void OnRep_UpdateIsActive();
};
