// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/ParaboleProjectileStat/PrabolProjectileStat.h"
#include "Curves/CurveVector.h"
#include "CurveProjectileStat.generated.h"

/**
 *
 */
UCLASS()
class ROBOTHUNTER_API UCurveProjectileStat : public UPrabolProjectileStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Parabole|Curve")
	TObjectPtr<UCurveVector> projectileCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Parabole|Curve")
	TObjectPtr<UCurveFloat> speedCurve = nullptr;


public:
	UCurveProjectileStat();

#pragma region Getter
	FORCEINLINE UCurveVector* GetProjectileCurve() const 
	{
		return projectileCurve; 
	}
	FORCEINLINE UCurveFloat* GetSpeedCurve() const 
	{
		return speedCurve;
	}

#pragma endregion

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
