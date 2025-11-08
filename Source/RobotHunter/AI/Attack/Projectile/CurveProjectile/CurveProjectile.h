// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/CurveProjectileStat/CurveProjectileStat.h"
#include "Curves/CurveVector.h"
#include "CurveProjectile.generated.h"

/**
 *
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API ACurveProjectile : public AProjectile
{
	GENERATED_BODY()
	UPROPERTY()
	TObjectPtr<UCurveProjectileStat> curveProjectileStat = nullptr;
	UPROPERTY()
	TObjectPtr<UCurveVector> curveProjectile = nullptr;
	UPROPERTY()
	TObjectPtr<UCurveFloat> speedCurve = nullptr;

	float currentTime = 0;
	float totalTime = 0;

	float currentSpeed = 0;
	float mynSpeed = 0;

	bool isEndMove = false;

	FVector direction, lastPosition;

	FTimerHandle testTimer;

public:
	FORCEINLINE UCurveProjectileStat* GetCurveProjectileStats()const
	{
		return curveProjectileStat;
	}

protected:
	virtual void BeginPlay()override;
	virtual void InitProjectile(const FVector& _targetLocation) override;
	void InitSpeed();
	virtual void Tick(float _deltaTime)override;

private:
#pragma region Move
	void Move();
	void MoveCurve();
	void UpdatePositionCurve(const FVector& _newLocation);

protected:
	virtual void EndMove();
	virtual void StopMove();
#pragma endregion

	void UpdateSpeed();
};
