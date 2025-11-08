// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/LineProjectileStat/LineProjectileStat.h"
#include "LineProjectile.generated.h"

#define LINE_PROJECTILE_USE_LOG false

/*
* The line projectile class of AI's distance attack
*/
UCLASS(HideDropdown)
class ROBOTHUNTER_API ALineProjectile : public AProjectile
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	FVector direction;

protected:
	UPROPERTY()
	TObjectPtr<ULineProjectileStat> lineProjectileStat = nullptr;

public:
#pragma region Getter
	FORCEINLINE FVector GetDirection() const 
	{
		return direction; 
	}
	FORCEINLINE ULineProjectileStat* GetLineProjectileStat()
	{
		return lineProjectileStat; 
	}
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetDirection(const FVector& _direction) 
	{
		direction = _direction;
	}
#pragma endregion


	/// <summary>
	/// Move to Line
	/// </summary>
	void MoveToLine();

	virtual void InitProjectile(const FVector& _targetLocation) override;

protected:
	virtual void Tick(float _deltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
