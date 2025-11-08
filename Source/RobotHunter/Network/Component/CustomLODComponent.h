// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "CustomLODComponent.generated.h"

// Custom Component optimizing network performance like tick rates and update frequencies based on distance
UCLASS(BLueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UCustomLODComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Parameter_Properties
	// It's FPS MAX in network
	UPROPERTY(EditAnywhere, Category = "Parameter|NetUpdateFrequency")
	float netUpdateFrequencyMax = 100;

	// It's FPS MIN in network
	UPROPERTY(EditAnywhere, Category = "Parameter|NetUpdateFrequency")
	float netUpdateFrequencyMin = 10;
	
	// Change the tick update interval
	UPROPERTY(EditAnywhere, Category = "Parameter|UpdateWithTick")
	float updateWithTickInterval = 0.5f;

	// Enable or disable debugging
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool isDebug = false;

	// Determines if lag simulation is enabled
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool isLag = true;

	// Distance = Distance + TrackingRange  (Distance max for Net Update Frequency MAX)
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float distance = 2000;
#pragma endregion

#pragma region Pointer_Properties
	// The arena associated with this component
	UPROPERTY()
	AArena* arena = nullptr;

	// Reference to the custom game state
	UPROPERTY()
	ACustomGameState* gameState = nullptr;
#pragma endregion

	bool isInitializationGood = false;

public:	
	/// Constructor
	UCustomLODComponent();

	FORCEINLINE void SetDistance(const float _distance) { distance = _distance; }
	FORCEINLINE float GetDistance() const { return distance; }

protected:
	/// Initializes references to the arena and game state, and sets initial tick rate and update frequency.
	virtual void BeginPlay() override;

public:	
	/// Updates based on characters' distances and optionally visualizes debug shapes.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DrawDebug();

	/// Adjusts the tick interval for AI components in the arena based on lag conditions.
	/// @param _isLag Determines whether lag conditions are active.
	void TickRate(const bool _isLag);

	/// Updates the network refresh rate for AI components in the arena to optimize performance.
	/// @param _isLag Enables or disables lag simulation based on distance.
	void UpdateFrequency(const bool _isLag);

	/// Calculates the distance between characters and adjusts the tick rate dynamically.
	/// Ensures LOD logic manages tick behavior for AI components under specific distance thresholds.
	void DistanceCharacterTick();

	/// Modifies network frequencies based on the distance between characters.
	/// Dynamically adjusts for larger distances to reduce bandwidth usage.
	void DistanceCharacterNetworkUpdate();
};
