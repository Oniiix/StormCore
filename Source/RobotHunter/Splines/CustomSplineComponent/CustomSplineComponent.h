// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "CustomSplineComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomSplineComponent : public USplineComponent
{
	GENERATED_BODY()
	int numberPoint = 0;
	bool canAddPoint = true;

	TObjectPtr<class AToolNavLink> tool = nullptr;

public:
	UCustomSplineComponent();
	FORCEINLINE int GetNumberPoint()const
	{
		return numberPoint;
	}
	FORCEINLINE void SetCanAddPoint(const bool _canAddPoint)
	{
		canAddPoint = _canAddPoint;
	}

protected:
	virtual void BeginPlay() override;
	virtual void UpdateSpline()override;
	void AddPoint();
};
