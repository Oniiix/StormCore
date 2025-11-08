

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/ProjectileStats.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "BlastComponent.generated.h"

class AProjectile;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROBOTHUNTER_API UBlastComponent : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlast, const FVector&, _actorLocation, const FHitResult&, _hitResult);
	FOnBlast onBlast;

	TObjectPtr< AProjectile> owner = nullptr;
	TObjectPtr<AArena> arena = nullptr;

	float lifePanTrigger = 0;
	float damage = 0;
	float sphereRadius = 0;
	bool isFloor = false;

	TObjectPtr< UProjectileStats> statsProjectile = nullptr;

	TArray<TEnumAsByte< ECollisionChannel>> collisionBlast;

public:	
	UBlastComponent();

	FORCEINLINE FOnBlast& OnBlast()
	{
		return onBlast;
	}

	FORCEINLINE void SetLifePanTrigger(const float _lifePan)
	{
		lifePanTrigger = _lifePan;
	}
	FORCEINLINE void SetDamage(const float _damage)
	{
		damage = _damage;
	}
	FORCEINLINE void SetSphereRadius(const float _sphereRadius)
	{
		sphereRadius = _sphereRadius;
	}
	FORCEINLINE void SetLayerBlast(const TArray<TEnumAsByte< ECollisionChannel>>& _layerBlast)
	{
		collisionBlast = _layerBlast;
	}
	FORCEINLINE void SetArena(AArena* _arena)
	{
		arena = _arena;
	}
public:
	void Init(UProjectileStats* _stats);

private:
	UFUNCTION() void ActivateBlast(const FVector& _actorLocation, const FHitResult& _result);
};
