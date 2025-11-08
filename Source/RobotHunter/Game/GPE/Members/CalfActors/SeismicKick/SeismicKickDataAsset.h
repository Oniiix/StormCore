

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SBoxActor.h"
#include "StoneActor.h"
#include "SeismicKickDataAsset.generated.h"
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USeismicKickDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "SeismicKick", meta = (Units = "Seconds", UMin = 0.01f, ClampMin = 0.01f))
	float maxChargeDuration = 3;
	UPROPERTY(EditAnywhere, Category = "SeismicKick", meta = (Units = "Seconds", UMin = 0.01f, ClampMin = 0.01f))
	float minChargeDuration = 0.5f;
	UPROPERTY(EditAnywhere, Category = "SeismicKick", meta = (Units = "Centimeters"))
	float offset = 50;
	UPROPERTY(EditAnywhere, Category = "SeismicKick", meta = (Units = "Centimeters"))
	float minLength = 200;
	UPROPERTY(EditAnywhere, Category = "SeismicKick", meta = (Units = "Centimeters"))
	float maxLength = 600;
	UPROPERTY(EditAnywhere, Category = "SeismicKick")
	FVector boxExtent = FVector(100);
	UPROPERTY(EditAnywhere, Category = "SeismicKick")
	TSubclassOf<ASBoxActor> boxRef = nullptr;



	UPROPERTY(EditAnywhere, Category = "SBox|Movement")
	TArray<TEnumAsByte<EObjectTypeQuery>> boxSnapLayer;
	UPROPERTY(EditAnywhere, Category = "SBox|Movement", meta = (Units="s", UMin = 0, ClampMin = 0))
	float timeBetweenGap = 0.5f;
	UPROPERTY(EditAnywhere, Category = "SBox|Movement", meta = (ClampMin = 0.01f, UMin = 0.1f))
	float minBoxSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "SBox|Movement", meta = (ClampMin = 0.01f, UMin = 0.1f))
	float maxBoxSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = "SBox|Movement")
	float diffBoxSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "SBox|Bump", meta = (UMin = 0, ClampMin = 0, UMax = 180, ClampMax = 180, Units = "Degrees"))
	float ejectionAngle = 45;	
	UPROPERTY(EditAnywhere, Category = "SBox|Bump", meta = (UMin = 0, ClampMin = 0))
	float ejectionForce = 1;
	UPROPERTY(EditAnywhere, Category = "SBox|Bump", meta = (UMin = 0, ClampMin = 0))
	int damage = 30;

public:
	//Weapon
	FORCEINLINE float GetMaxChargeDuration() const { return maxChargeDuration; }
	FORCEINLINE float GetMinChargeDuration() const { return minChargeDuration; }
	FORCEINLINE float GetMinLength() const { return minLength; }
	FORCEINLINE float GetOffset() const { return offset; }
	FORCEINLINE float GetMaxLength() const { return maxLength; }
	FORCEINLINE FVector GetBoxExtent() const { return boxExtent; }
	FORCEINLINE TSubclassOf<ASBoxActor> GetBoxRef() const { return boxRef; }

	//SBox
	FORCEINLINE TArray<TEnumAsByte<EObjectTypeQuery>> GetBoxSnapLayer() const { return boxSnapLayer; }
	FORCEINLINE float GetTimeBetweenGap() const { return timeBetweenGap; }
	FORCEINLINE float GetMinBoxSpeed() const { return minBoxSpeed; }
	FORCEINLINE float GetMaxBoxSpeed() const { return maxBoxSpeed; }
	FORCEINLINE float GetDiffBoxSpeed() const { return diffBoxSpeed; }

	FORCEINLINE float GetEjectionAngle() const { return ejectionAngle; }
	FORCEINLINE float GetEjectionForce() const { return ejectionForce; }
	FORCEINLINE int GetDamage() const { return damage; }

protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
