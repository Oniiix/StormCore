// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Transmitter/TransmitterActor.h"
#include "Components/BillboardComponent.h"
#include <Components/SphereComponent.h>
#include "ScannerActor.generated.h"

/**
 * 
 */
UCLASS(ShowCategories = ("Interaction"))
class ROBOTHUNTER_API AScannerActor : public ATransmitterActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> interactPoint = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> sphereInteractRange = nullptr;

	UPROPERTY(EditAnywhere, Category="Custom|Scanner", meta = (UMin = 1, ClampMin = 1, UMax = 360, ClampMax = 360))
	int16 interactableAngle = 180;

	UPROPERTY(EditAnywhere, Category = "Custom|Scanner", meta = (UMin = 0.0f, ClampMin = 0.0f))
	float interactionTime;

	TObjectPtr<AMainCharacter> player = nullptr;

	FVector2D interactPointPosition;
	bool movePlayer = false;
	bool rotatePlayer = false;

public:
	AScannerActor();

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float _deltaTime)override;

public:
	virtual void Interacted(AMainCharacter* _character) override;

private:
	void MovePlayerToPoint();

	void RotatePlayerToPoint();

	UFUNCTION() void LaunchScannerAnimation();
	UFUNCTION() void UseScanner();


	void DrawDebug();

};
