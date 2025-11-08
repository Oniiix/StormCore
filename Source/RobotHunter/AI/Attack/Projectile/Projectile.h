// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileStat/ProjectileStats.h"
#include "RobotHunter/AI/Attack/Projectile/BlastComponent/BlastComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/TriggerSphere.h"
#include "Projectile.generated.h"

/*
* The base projectile class of AI's distance attack
*/
UCLASS(ABSTRACT)
class ROBOTHUNTER_API AProjectile : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExplode, const FHitResult&, _normal);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowprivateAccess))
	FOnExplode onExplode;

protected:
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	TObjectPtr<UProjectileStats> stats = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	TObjectPtr<USphereComponent> projectileCollision = nullptr;

	UPROPERTY(Replicated)
	FVector targetLocation = FVector::ZeroVector;
	UPROPERTY(Replicated)
	FVector startLocation = FVector::ZeroVector;

	TObjectPtr<UBlastComponent> blastComponent = nullptr;

	bool canMove = true;
	float speed;
	bool useBlast = false;
	FHitResult* result;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	TArray<TEnumAsByte< EObjectTypeQuery>> layer;
public:
	AProjectile();

	FORCEINLINE FOnExplode OnExplode()
	{
		return onExplode;
	}


	FORCEINLINE UProjectileStats* GetStats() const 
	{
		return stats; 
	}

	FORCEINLINE void SetTargetLocation(const FVector& _targetLocation) 
	{
		targetLocation = _targetLocation;
	}
	FORCEINLINE void InitClearArena(AArena* _arena)
	{
		_arena->OnClearArena().AddDynamic(this, &AProjectile::ClearArena);
		if (blastComponent)
			blastComponent->SetArena(_arena);
	}
	/// <summary>
	/// InitProjectile
	/// </summary>
	virtual void InitProjectile(const FVector& _targetLocation);

protected:
	virtual void BeginPlay() override;
	virtual void SetActorLocationProjectile(const FVector& _newLocation);

public:
	UFUNCTION() void ClearArena(AAlienDoor* _exitGate, bool _giveBonus);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UFUNCTION() void OnOverlap(FHitResult _actorResult);
};
