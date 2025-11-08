// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/ProjectileStats.h"
#include "PrabolProjectileStat.generated.h"

UCLASS()
class ROBOTHUNTER_API UPrabolProjectileStat : public UProjectileStats
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Projectile|Parabole",meta =(EditCondition = "useCurveType", EditConditionHides))
	TEnumAsByte<ECurveType> curveType = ECurveType::VERTICAL;

	UPROPERTY(EditAnywhere, Category = "Projectile|Parabole", meta = (UMin = 0.1f, ClampMin = 0.1f))
	float topCurve = 500;

	UPROPERTY(EditAnywhere, Category = "Projectile|Parabole", meta = (UMin = 1, ClampMin = 1))
	int definitionCurve = 10;

	UPROPERTY()
	bool useCurveType = true;

public:
	FORCEINLINE float GetTopCurve() const 
	{
		return topCurve;
	}
	FORCEINLINE int GetDefinitionCurve() const 
	{
		return definitionCurve;
	}
	FORCEINLINE TEnumAsByte<ECurveType> GetCurveType() const 
	{
		return curveType;
	}
};
