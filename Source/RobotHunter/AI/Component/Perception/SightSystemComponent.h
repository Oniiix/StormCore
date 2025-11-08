// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerceptionSystemComponent.h"
#include "SightSystemComponent.generated.h"

/**
 * The Sight system of AI to detect player by simulated vision
 */
UCLASS()
class ROBOTHUNTER_API USightSystemComponent : public UPerceptionSystemComponent
{
	GENERATED_BODY()

#pragma region Properties
	/// Sight
	UPROPERTY(EditAnywhere, Category = "Perception_Property|Sight", meta = (EditCondition = "!disablePerception", Min = 0, Max = 360, ClampMin = 0, ClampMax = 360))
	int sightAngle = 90;
	UPROPERTY(EditAnywhere, Category = "Perception_Property|Sight", meta = (EditCondition = "!disablePerception"))
	TArray<TEnumAsByte<EObjectTypeQuery>> layersObstacle = {};

	int currentRangePerception = 0;
#pragma endregion

#pragma region Methods
public:
	USightSystemComponent();

	/// Getter
	FORCEINLINE TArray<TEnumAsByte<EObjectTypeQuery>> GetLayersObstacles() const { return layersObstacle; }
	/// Setter
	FORCEINLINE void SetSightAngle(const int _sightAngle) { sightAngle = _sightAngle; }
	FORCEINLINE void SetLayersObstacle(const TArray<TEnumAsByte<EObjectTypeQuery>>& _layersObstacle) { layersObstacle = _layersObstacle; }

	void InitPerception() override;

private:
	/// <summary>
	/// The behaviour of Sight perception
	/// </summary>
	void PerceptionBehaviour() override;
	/// <summary>
	/// Get the angle between two vector
	/// </summary>
	/// <param name="_u"> vector 1 </param>
	/// <param name="_v"> vector 2 </param>
	/// <returns> angle result </returns>
	float GetVectorAngle(const FVector& _u, const FVector& _v) const;
	/// <summary>
	/// Draw the debug of sight perception
	/// </summary>
	bool DrawDebug() override;
#pragma endregion
};
