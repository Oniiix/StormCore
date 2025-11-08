// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StateTreeReference.h"
#include "RobotHunter/AI/Component/Attack/ThreatReasonDataAsset.h"
#include "AIsSettingsDataAsset.generated.h"

class AAI_Base;

USTRUCT()
struct FRangeAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERangeAttack> range = ERangeAttack::Mid;
	UPROPERTY(EditAnywhere, meta = (UMin = 0.1f, ClampMin = 0.1f))
	float cooldown = 1.f;
};

/**
 * The data asset that store all settings of AIs
 */
UCLASS()
class ROBOTHUNTER_API UAIsSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

#pragma region Properties
#pragma region StateTree
	UPROPERTY(EditAnywhere, Category = "StateTree")
	FStateTreeReference stateTreeRef;
#pragma endregion

#pragma region Attack
	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<FRangeAttack> mapRange;
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool disableAttack = false;
	UPROPERTY(EditAnywhere, Category = "Attack|CQC", meta = (UMin = 0.1f, ClampMin = 0.1f, UMax = 0.9f, ClampMax = 0.9f))
	float pushSnapPower = 0.5;
	UPROPERTY(EditAnywhere, Category = "Attack|CQC", meta = (UMin = 50, ClampMin = 50))
	int moveDistance = 200;
	UPROPERTY(EditAnywhere, Category = "Attack|CQC")
	FVector2D imprecisionFactor = FVector2D(1.f, 1.f);
#pragma endregion

#pragma region Life
	UPROPERTY(EditAnywhere, Category = "Life", meta = (UMin = 0, ClampMin = 0))
	int maxLife = 100;
	UPROPERTY(EditAnywhere, Category = "Life", meta = (UMin = 0, ClampMin = 0))
	int startLife = 100;
	UPROPERTY(EditAnywhere, Category = "Life", meta = (UMin = 1.f, ClampMin = 1.f))
	float weakPointDamageMultiplier = 2.f;
#pragma endregion

#pragma region Movement
	UPROPERTY(EditAnywhere, category = "Movement", meta = (UMin = 0, ClampMin = 0))
	int walkSpeed = 400;
	UPROPERTY(EditAnywhere, category = "Movement", meta = (UMin = 0, ClampMin = 0))
	int runSpeed = 800;
	UPROPERTY(EditAnywhere, category = "Movement", meta = (UMin = 0, ClampMin = 0))
	int acceleration = 1000;
	UPROPERTY(EditAnywhere, category = "Movement", meta = (UMin = 0.1f, ClampMin = 0.1f))
	float rotationSpeed = 5.f;
	UPROPERTY(EditAnywhere, category = "Movement")
	bool drawDebugMove = false;
	UPROPERTY(EditAnywhere, category = "Movement")
	TArray<TEnumAsByte<EObjectTypeQuery>> layersGround;
	UPROPERTY(EditAnywhere, Category = "Movement/Slow", meta = (UMin = 0, ClampMin = 0))
	float slowedDelay = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Movement/Slow", meta = (UMin = 0, ClampMin = 0, UMax = 100, ClampMax = 100))
	float maxSlow = 50.f;
	UPROPERTY(EditAnywhere, Category = "Movement/Slow", meta = (UMin = 0, ClampMin = 0))
	float maxDamageSlow = 50.f;
#pragma endregion

#pragma region Tracking
	UPROPERTY(EditAnywhere, Category = "Tracking")
	bool disableTracking = false;
	UPROPERTY(EditAnywhere, Category = "Tracking", meta = (EditCondition = "!disableTracking"))
	bool drawDebugTracking = false;
#pragma endregion
#pragma endregion

#pragma region Methods
public:
	/// StateTree
	FORCEINLINE FStateTreeReference GetStateTreeRef() { return stateTreeRef; }
	/// Attack
	FORCEINLINE TArray<FRangeAttack> const GetListRange() { return mapRange; }
	FORCEINLINE bool GetDisableAttack() { return disableAttack; }
	FORCEINLINE float GetPushSnapPower() { return pushSnapPower; }
	FORCEINLINE int GetMoveDistance() { return moveDistance; }
	FORCEINLINE FVector2D GetImprecisionFactor() { return imprecisionFactor; }
	/// Life
	FORCEINLINE int GetMaxLife() const { return maxLife; }
	FORCEINLINE int GetStartLife() const { return startLife; }
	FORCEINLINE float GetWeakPointDamageMultiplier() const { return weakPointDamageMultiplier; }
	/// Movement
	FORCEINLINE int GetWalkSpeed() const { return walkSpeed; }
	FORCEINLINE int GetRunSpeed() const { return runSpeed; }
	FORCEINLINE int GetAcceleration() const { return acceleration; }
	FORCEINLINE float GetRotationSpeed() const { return rotationSpeed; }
	FORCEINLINE bool GetDrawDebugMove() const { return drawDebugMove; }
	FORCEINLINE float GetSlowedDelay() const { return slowedDelay; }
	FORCEINLINE float GetMaxSlow() const { return maxSlow; }
	FORCEINLINE float GetMaxDamageSlow() const { return maxDamageSlow; }
	FORCEINLINE TArray<TEnumAsByte<EObjectTypeQuery>> GetLayersGround() const { return layersGround; }
	/// Tracking
	FORCEINLINE bool GetDisableTracking() const { return disableTracking; }
	FORCEINLINE bool GetDrawDebugTracking() const { return drawDebugTracking; }

	FORCEINLINE bool CheckAIsSettings() const { return stateTreeRef.IsValid() && !mapRange.IsEmpty(); }

private:
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion
};
