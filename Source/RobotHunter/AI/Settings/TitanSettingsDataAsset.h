
#pragma once
#include "CoreMinimal.h"
#include "AIsSettingsDataAsset.h"
#include "TitanSettingsDataAsset.generated.h"

UCLASS()
class ROBOTHUNTER_API UTitanSettingsDataAsset : public UAIsSettingsDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Titan", meta = (Umin = 0.1f, ClampMin = 0.1f, UMax = 1.f, ClampMax = 1.f))
	float frontLocationModifier = 0.75f;
	UPROPERTY(EditAnywhere, Category = "Titan")
	TArray<TEnumAsByte<EObjectTypeQuery>> frontLayers;
	UPROPERTY(EditAnywhere, Category = "Titan", meta = (Umin = 10, ClampMin = 10))
	int damageThreshold = 200;
	UPROPERTY(EditAnywhere, Category = "Titan", meta = (Umin = 0.1f, ClampMin = 0.1f))
	float delayThreshold = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Titan", meta = (Umin = 0, ClampMin = 0))
	int shieldSpeed = 300;
	UPROPERTY(EditAnywhere, Category = "Titan", meta = (Umin = 0.1f, ClampMin = 0.1f))
	float shieldDisableDelay = 3.f;

public:
	FORCEINLINE float GetFrontLocationModifier() const { return frontLocationModifier; }
	FORCEINLINE	TArray<TEnumAsByte<EObjectTypeQuery>> GetFrontLayers() const { return frontLayers; }
	FORCEINLINE int GetDamageThreshold() const { return damageThreshold; }
	FORCEINLINE float GetDelayThreshold() const { return delayThreshold; }
	FORCEINLINE int GetShieldSpeed() const { return shieldSpeed; }
	FORCEINLINE float GetShieldDisableDelay() const { return shieldDisableDelay; }
};
