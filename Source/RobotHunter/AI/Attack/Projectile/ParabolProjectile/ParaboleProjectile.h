// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/ParaboleProjectileStat/PrabolProjectileStat.h"
#include "ParaboleProjectile.generated.h"

#define PARABOLE_PROJECTILE_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AParaboleProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	TArray<FVector> allPoints;
	UPROPERTY()
	TObjectPtr<UPrabolProjectileStat> paraboleStat = nullptr;
	UPROPERTY()
	FVector middleCurve;

	int index;

public:
#pragma region Getter
	FORCEINLINE float GetIndex()const 
	{
		return index;
	}
	FORCEINLINE TArray<FVector> GetAllPoints()const 
	{
		return allPoints;
	}
	FORCEINLINE UPrabolProjectileStat* GetParaboleStat() 
	{
		return paraboleStat; 
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// <param name="_allPoints"></param>
	/// Set and Replic the allPoints
	/// </summary>
	/// <param name="_allPoints"></param>
	FORCEINLINE void SetAllPoints(const TArray<FVector>& _allPoints) 
	{
		allPoints = _allPoints; 
	}
	/// <summary>
	/// Set Middle Curve
	/// </summary>
	/// <param name="_goalLocation"> end Position </param>
	/// <param name="_baseLocation"> start position </param>
	void SetMiddleCurve(const FVector& _goalLocation, const FVector& _baseLocation);
#pragma endregion

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float _deltaTime)override;
	/// <summary>
	/// Init Curve
	/// </summary>
	virtual void InitProjectile(const FVector& _targetLocation) override;

public:
#pragma region Move
	/// <summary>
	/// Move to Parabole
	/// </summary>
	void MoveToParabole();
protected:
	/// <summary>
	/// Call when the movement is Finish
	/// </summary>
	virtual void EndMove();
#pragma endregion


#pragma region Curve
private:
	/// <summary>
	/// Create the list points to curve
	/// </summary>
	/// <param name="_goalPosition"> end Position </param>
	/// <returns> list of points of curve </returns>
	TArray<FVector> CreateCurve(const FVector& _goalPosition);
	/// <summary>
	/// Return a point in curve
	/// </summary>
	/// <param name="_pos1">: origin</param>
	/// <param name="_pos2">: middle curve</param>
	/// <param name="_pos3">: destination</param>
	/// <param name="_time">Time in Lerp</param>
	/// <returns> FVector Position in curve </returns>
	FVector GetMovementCurve(const FVector& _pos1, const FVector& _pos2, const FVector& _pos3, const float _time)const;
#pragma endregion

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
