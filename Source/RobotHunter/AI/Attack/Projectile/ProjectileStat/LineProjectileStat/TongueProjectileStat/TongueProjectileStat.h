// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/LineProjectileStat/LineProjectileStat.h"
#include "TongueProjectileStat.generated.h"

UCLASS()
class ROBOTHUNTER_API UTongueProjectileStat : public ULineProjectileStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "TongueProjectileStat_Property")
	float lengthTongue;
	UPROPERTY(EditAnywhere, Category = "TongueProjectileStat_Property")
	float speedTongueBack;
	UPROPERTY(EditAnywhere, Category = "TongueProjectileStat_Property")
	float delayWait;
	UPROPERTY(EditAnywhere, Category = "TongueProjectileStat_Property")
	float forcePush;

public:
	FORCEINLINE float GetLengthTongue() const { return lengthTongue; }
	FORCEINLINE float GetSpeedTongueBack() const { return speedTongueBack; }
	FORCEINLINE float GetDelayWait() const { return delayWait; }
	FORCEINLINE float GetForcePush() const { return forcePush; }
};
