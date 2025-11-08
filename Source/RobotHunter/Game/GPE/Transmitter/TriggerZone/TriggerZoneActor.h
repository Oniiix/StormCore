// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Transmitter/TransmitterActor.h"
#include <Components/ShapeComponent.h>
#include "RobotHunter/Utils/ENUM_TriggerZone.h"
#include "TriggerZoneActor.generated.h"


UCLASS()
class ROBOTHUNTER_API ATriggerZoneActor : public ATransmitterActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<UShapeComponent> trigger = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	ETriggerTypeActivation typeActivation = ETriggerTypeActivation::ENTER_ZONE;
	UPROPERTY(EditInstanceOnly, Category = "Custom|Property")
	EShapeType shapeType = EShapeType::SHAPE_NONE;


protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UFUNCTION() void OnEnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
