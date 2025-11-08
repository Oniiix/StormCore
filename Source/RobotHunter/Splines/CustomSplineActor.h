#pragma once

#include "CoreMinimal.h"
#include "../CustomActor.h"
#include "Components/SplineComponent.h"
#include "CustomSplineActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ACustomSplineActor : public ACustomActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Spline", BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<USplineComponent> spline = nullptr;

	UPROPERTY(EditAnywhere, Category = "Spline|Snap")
	TArray<TEnumAsByte<EObjectTypeQuery>> snapMask;

	UPROPERTY(EditAnywhere, Category = "Spline|Tool|Snap")
	FVector snapOffset = FVector(0, 0, 10);

	UPROPERTY(EditAnywhere, Category = "Spline|Tool|Points")
	bool usePointsToolDebug;

	UPROPERTY(EditAnywhere, Category = "Spline|Tool|Points")
	bool useCustomDistance;

	UPROPERTY(EditAnywhere, Category = "Spline|Tool|Points", meta = (EditCondition = "useCustomDistance"))
	float distanceBetweenPoints;

	UPROPERTY(EditAnywhere, Category = "Spline|Tool|Points")
	float numberOfPoints;

public:
	FORCEINLINE int GetNumberOfPoints() const { return spline ? spline->GetNumberOfSplinePoints() : 0; }
	FORCEINLINE float GetLength() const { return spline ? spline->GetSplineLength() : 0.0f; }
	FORCEINLINE FVector GetLocationAtDistance(const float _distance) const
	{
		return spline ? spline->GetLocationAtDistanceAlongSpline(_distance, ESplineCoordinateSpace::World) : FVector(0.0f);
	}
	FORCEINLINE FVector GetLocationAtPointIndex(const int _index) const
	{
		return spline ? spline->GetLocationAtSplineInputKey(_index, ESplineCoordinateSpace::World) : FVector(0.0);
	}
	FORCEINLINE float GetDistanceAtLocation(const FVector& _location) const
	{
		return spline ? spline->GetDistanceAlongSplineAtLocation(_location, ESplineCoordinateSpace::World) : 0.0f;
	}
	FORCEINLINE TObjectPtr<USplineComponent> GetSpline() const { return spline; }

public:
	ACustomSplineActor();

protected:
	virtual void DrawDebug() override;

public:
	UFUNCTION(CallInEditor/*, Category = "Spline|Snap"*/) virtual void SnapSplinePoint();
	UFUNCTION(CallInEditor/*, Category = "Spline|Points"*/) virtual void RefreshPoints();

protected:
	void RefreshPointSpline(USplineComponent* _spline);
	virtual void SnapSpline(USplineComponent* _spline);
};
