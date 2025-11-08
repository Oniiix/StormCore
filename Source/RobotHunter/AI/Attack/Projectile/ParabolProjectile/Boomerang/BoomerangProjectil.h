// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ParaboleProjectile.h"
#include "BoomerangProjectil.generated.h"

/**
 *
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API ABoomerangProjectil : public AParaboleProjectile
{
	GENERATED_BODY()

	FVector middle;
	bool returnMode = false;
	int endIndex = 0;

public:
#pragma region Setter
	FORCEINLINE void SetMiddle(const FVector& _middle) { middle = _middle; }
#pragma endregion

	/// <summary>
	/// Init middle
	/// </summary>
	virtual void InitProjectile(const FVector& _targetLocation) override;

protected:
#pragma region Return curve
	/// <summary>
	/// Control ReturnPoint
	/// </summa>ry
	virtual void EndMove() override;

private:
	/// <summary>
	/// Create curve return
	/// </summary>
	void SetReturnPoint();
#pragma endregion

	virtual void NotifyActorBeginOverlap(AActor* _actor) override;
};
