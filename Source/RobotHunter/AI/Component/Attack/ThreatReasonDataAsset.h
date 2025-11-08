// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "ThreatReasonDataAsset.generated.h"

USTRUCT()
struct FThreatType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (UMIn = 0, ClampMin = 0))
	float valueMin = 0;
	UPROPERTY(EditAnywhere, meta = (UMIn = 0, ClampMin = 0))
	float valueMax = 1;
	UPROPERTY(EditAnywhere, meta = (UMIn = 0, ClampMin = 0, UMax = 100, ClampMax = 100))
	int weight = 0;

	FThreatType() = default;

	FORCEINLINE float GetPercentNormalizedValueThreat(const float _valueThreat) const { return FMath::Clamp((_valueThreat - valueMin) / (valueMax - valueMin), 0.0f, 1.0f) * weight; }
};

UCLASS()
class ROBOTHUNTER_API UThreatReasonDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FThreatType damageOnIA;
	UPROPERTY(EditAnywhere)
	FThreatType targetLife;
	UPROPERTY(EditAnywhere)
	FThreatType distanceTargetToAI;
	UPROPERTY(EditAnywhere)
	FThreatType paralarvaOnPlayer;
	UPROPERTY(EditAnywhere)
	FThreatType pennantOnPlayer;
	UPROPERTY(EditAnywhere)
	FThreatType trapperOnPlayer;
	UPROPERTY(EditAnywhere)
	FThreatType torpedoOnPlayer;
	UPROPERTY(EditAnywhere)
	FThreatType titanOnPlayer;

public:
	FORCEINLINE FThreatType GetDamageOnIA() const { return damageOnIA; }
	FORCEINLINE FThreatType GetTargetLife() const { return targetLife; }
	FORCEINLINE FThreatType GetDistanceTargetToAI() const { return distanceTargetToAI; }
	FORCEINLINE FThreatType GetParalarvaOnPlayer() const { return paralarvaOnPlayer; }
	FORCEINLINE FThreatType GetPennantOnPlayer() const { return pennantOnPlayer; }
	FORCEINLINE FThreatType GetTrapperOnPlayer() const { return trapperOnPlayer; }
	FORCEINLINE FThreatType GetTorpedoOnPlayer() const { return torpedoOnPlayer; }
	FORCEINLINE FThreatType GetTitanOnPlayer() const { return titanOnPlayer; }
	FORCEINLINE bool CheckThreatReason() const { return GetTotalWeight() > 0; }

private:
	int GetTotalWeight() const;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
