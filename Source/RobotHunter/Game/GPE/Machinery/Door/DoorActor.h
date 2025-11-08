// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "DoorActor.generated.h"

/**
 * 
 */
UCLASS(HideCategories = ("Custom|ReceiverInterface"))
class ROBOTHUNTER_API ADoorActor : public AReceiver
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseDoor);
	FOnCloseDoor onCloseDoor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> rightMesh = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> leftMesh = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> baseLeft = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> baseRight = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float openTime = 5;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float closeTime = 5;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float maxAngle = 180;

	float openSpeed = 1.0f;
	float closeSpeed = 1.0f;

	bool hasAuthority = false;

	FTimerHandle timerStopMove;

public:
	ADoorActor();

	FORCEINLINE FOnCloseDoor& OnCloseDoor()
	{
		return onCloseDoor;
	}

#pragma region Getter
	FORCEINLINE float GetCloseTime() const 
	{ 
		return closeTime;
	}
	FORCEINLINE float GetOpenTime() const 
	{
		return openTime; 
	}
	FORCEINLINE bool GetIsOpen() const 
	{
		return isReceiverActive; 
	}
	FORCEINLINE FTimerHandle GetTimerStopMove()const
	{
		return timerStopMove;
	}
#pragma endregion

protected:
	virtual void InitReceiver() override;
	virtual void Tick(float _deltaTime)override;

public:
	/// <summary>
	/// Activate Door
	/// </summary>
	virtual void Action() override;

#pragma region Door
	/// <summary>
	/// Launch Open or close the door
	/// </summary>
	void InteractDoor();
	/// <summary>
	/// Move the mesh for open door
	/// </summary>
	void OpenDoor();
	/// <summary>
	/// Move the mesh for close door
	/// </summary>
	void CloseDoor();

	/// <summary>
	/// Stop Movement Door
	/// </summary>
	void StopMove();
#pragma endregion
};
