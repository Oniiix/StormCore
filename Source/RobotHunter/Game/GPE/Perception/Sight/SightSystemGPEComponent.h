// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Perception/PerceptionSystemGPEComponent.h"
#include "SightSystemGPEComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API USightSystemGPEComponent : public UPerceptionSystemGPEComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Sight")float sightAngle = 90;
	UPROPERTY(EditAnywhere, Category = "Sight")float rateProcessToClear = 0.000001f;
	UPROPERTY(EditAnywhere, Category = "Sight")TArray<TEnumAsByte<EObjectTypeQuery>> layersObstacle = {};
	UPROPERTY(EditAnywhere, Category = "Sight")bool activateConeDetection = false;

	FTimerHandle timerProcessToClear;
	bool tryToClear = false;
protected:
	virtual void PerceptionBehaviour() override;
	float GetVectorAngle(const FVector& _u, const FVector& _v) const;
	void ProccessToClear();
	UFUNCTION() void EndProccessToClear();
	virtual void DrawDebug() override;
private:
	void DrawCone();
};
