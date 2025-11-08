#pragma once

#include "CoreMinimal.h"
#include "LeftArmActor.h"
#include "../../Projectile/ProjectileActor.h"
#include "ProjectileLeftArmActor.generated.h"

UCLASS()
class ROBOTHUNTER_API AProjectileLeftArmActor : public ALeftArmActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectileActor> projectileType;
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<AProjectileActor> projectile;
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<AProjectileActor> lastProjectile;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TObjectPtr<USceneComponent> projectileSpawn;

	UPROPERTY(EditAnywhere, Category = "HitScan")
	float maxRange;

	UPROPERTY(EditAnywhere, Category = "HitScan")
	TArray<TEnumAsByte<EObjectTypeQuery>> hitScanLayer;

	FVector projectileTarget;

public:
	AProjectileLeftArmActor();
	FORCEINLINE TObjectPtr<AProjectileActor> GetProjectile() { return projectile; }
	FORCEINLINE TSubclassOf<AProjectileActor> GetProjectileType() { return projectileType; }

	UFUNCTION(BlueprintCallable) void LaunchProjectile();
private:
	void FindProjectileTarget();

	void SpawnProjectile(const float _currentChargeTime, const FVector& _projectileTarget);
	UFUNCTION(Server, Unreliable) void ServerRPC_SpawnProjectile(const float _currentChargeTime, const FVector& _projectileTarget);

protected:
	virtual void Destroyed() override;
	virtual void PressBehavior() override;
	virtual void ResetMember() override;
	UFUNCTION(CallInEditor) virtual void ReleaseBehavior() override;


	virtual void SetupMember() override;

	virtual void EndCooldown() override;
	void CreateProjectile();
};
