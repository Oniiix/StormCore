// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/ENUM_Projectile.h"
#include "RobotHunter/AI/Attack/Projectile/ParabolProjectile/Blast/TriggerProjectileSphere.h"
#include "RobotHunter/Utils/ENUM_AI.h"

#include "ProjectileStats.generated.h"

/**
 *
 */
UCLASS(ABSTRACT)
class ROBOTHUNTER_API UProjectileStats : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FString name = "Projectile";
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (UMin = 1, ClampMin = 1))
	float speed = 500;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (UMin = 0.1f, ClampMin = 0.1f))
	float lifeSpan = 3;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (UMin = 1, ClampMin = 1))
	int numberOfProjectile = 1;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (UMin = 0, ClampMin = 0))
	float damage = 10;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TEnumAsByte<EProjectileRange> projectileRange = EProjectileRange::MIDDLE_RANGE;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float rangeMax = 500;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TEnumAsByte<EAIAnimation> animationToLaunch = EAIAnimation::Null;

	UPROPERTY(EditAnywhere, Category = "Projectile|CoolDown", meta = (UMin = 0.1f, ClampMin = 0.1f, EditCondition = "numberOfProjectile > 1"))
	float delayBetweenProjectile = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Projectile|CoolDown", meta = (UMin = 0.1f, ClampMin = 0.1f))
	float cooldown = 3.f;


	UPROPERTY(EditAnywhere, Category = "Projectile|Precision")
	bool usePrecision = false;
	UPROPERTY(EditAnywhere, Category = "Projectile|Precision", meta = (EditCondition = "usePrecision", EditConditionHides, UMin = 0, ClampMin = 0))
	float initPrecision = 50;
	UPROPERTY(EditAnywhere, Category = "Projectile|Precision", meta = (EditCondition = "usePrecision", EditConditionHides, UMin = 0.1f, ClampMin = 0.1f))
	int maxPrecision = 100;
	UPROPERTY(EditAnywhere, Category = "Projectile|Precision", meta = (EditCondition = "usePrecision", EditConditionHides, UMin = 0, ClampMin = 0))
	float addPrecision = 10;


	UPROPERTY(EditAnywhere, Category = "Projectile|Option")
	bool isDestroyOnPlayer = true;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option", meta = (EditCondition = "!isDestroyOnPlayer", EditConditionHides))
	bool canDuringHit = false;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option", meta = (EditCondition = "!isDestroyOnPlayer && canDuringHit", EditConditionHides))
	float timeDuring = 1;


	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger")
	bool useTrigger = false;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger", meta = (EditCondition = "useTrigger", EditConditionHides))
	float heightSphere = 100;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger", meta = (EditCondition = "useTrigger", EditConditionHides))
	TArray< TEnumAsByte<ECollisionChannel>> layerActivate;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger", meta = (EditCondition = "useTrigger", EditConditionHides))
	TSubclassOf<ATriggerProjectileSphere> triggerRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger", meta = (EditCondition = "useTrigger", EditConditionHides))
	TArray< TEnumAsByte<ECollisionChannel>> layerTrigger;

	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger", meta = (EditCondition = "useTrigger", EditConditionHides))
	bool isOnFloor = true;
	UPROPERTY(EditAnywhere, Category = "Projectile|Option|Trigger", meta = (EditCondition = "isOnFloor && useTrigger", EditConditionHides))
	TArray<TEnumAsByte< EObjectTypeQuery>> layerFloor;

public:
#pragma region Getter
	FORCEINLINE float GetSpeed() const 
	{
		return speed; 
	}
	FORCEINLINE float GetLifeSpan() const 
	{
		return lifeSpan; 
	}
	FORCEINLINE int GetNumberOfProjectile() const 
	{
		return numberOfProjectile; 
	}	
	FORCEINLINE float GetDamage() const 
	{
		return damage;
	}
	FORCEINLINE EProjectileRange GetProjectileRange() const 
	{
		return projectileRange; 
	}
	FORCEINLINE FString GetProjectileName() const 
	{
		return name; 
	}
	FORCEINLINE bool GetIsDestroyOnPlayer()const
	{
		return isDestroyOnPlayer;
	}
	FORCEINLINE float GetRangeMax()const
	{
		return rangeMax;
	}
	FORCEINLINE EAIAnimation GetAnimation()const
	{
		return animationToLaunch;
	}
#pragma region Precision
	FORCEINLINE bool GetUsePrecision()const
	{
		return usePrecision;
	}
	FORCEINLINE float GetInitPrecision() const 
	{
		return initPrecision; 
	}
	FORCEINLINE int GetMaxPrecision() const 
	{ 
		return maxPrecision; 
	}
	FORCEINLINE float GetAddPrecision() const 
	{
		return addPrecision;
	}
#pragma endregion

#pragma region CoolDown
	FORCEINLINE float GetDelayBetweenProjectile() const 
	{
		return delayBetweenProjectile; 
	}
	FORCEINLINE float GetCooldown() const 
	{
		return cooldown; 
	}
#pragma endregion

#pragma region Trigger
	FORCEINLINE bool GetUseTrigger()const
	{
		return useTrigger;
	}
	FORCEINLINE float GetHeightSphere()const
	{
		return heightSphere;
	}
	FORCEINLINE bool GetIsOnFloor()const
	{
		return isOnFloor;
	}
	FORCEINLINE TArray<TEnumAsByte< EObjectTypeQuery>> GetLayerFloor()const
	{
		return layerFloor;
	}
	FORCEINLINE TArray< TEnumAsByte<ECollisionChannel>> GetLayerActivate()const
	{
		return layerActivate;
	}
	FORCEINLINE TArray< TEnumAsByte<ECollisionChannel>> GetCollisionTrigger()const
	{
		return layerTrigger;
	}
	FORCEINLINE TSubclassOf<ATriggerProjectileSphere> GetTriggerRef()const
	{
		return triggerRef;
	}
#pragma endregion

#pragma region During
	FORCEINLINE bool GetCanDuring()const
	{
		return canDuringHit;
	}
	FORCEINLINE float GetTimeDuring()const
	{
		return timeDuring;
	}
#pragma endregion
#pragma endregion

	protected:
#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
