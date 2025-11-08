// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "RobotHunter/Game/GPE/Machinery/Door/DoorActor.h"
#include "SASActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ASASActor : public AReceiver
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	TArray<ADoorActor*> allDoors;

	UPROPERTY(EditAnywhere, Category = "Custom|Property", meta = (ArrayClamp = "allDoors"))
	int currentOpenedDoorIndex = 0;
	int lastOpenDoor = 0;

	FTimerHandle timerSAS;

	int numDoor = 0;

public:
	ASASActor();
	
protected:
	virtual void InitReceiver() override;
	void BindEvent();
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	/// <summary>
	/// Open door 
	/// </summary>
	UFUNCTION() void ActivateDoor();

private:
	UFUNCTION() void ActionSwitch();
	UFUNCTION() void ActionTimer();
	UFUNCTION() void ActionEndless();

	void OpenAllDoor();
	void CloseDoor();
};
