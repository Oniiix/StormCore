#pragma once

#include "CoreMinimal.h"
#include "../../../../CustomActor.h"
#include <Components/SphereComponent.h>
#include "RobotHunter/AI/Component/Life/lifeComponent.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "NailgunProjectile.generated.h"

/**
 * 
 */

#define NAIL_PROJECTILE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API ANailgunProjectile : public ACustomActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplode);


	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnExplode onExplode;

	UPROPERTY(Replicated)
	TObjectPtr<class ANailGunActor> nailGun = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float nailLifespan = 1.05f;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Explosion")
	float explosionDelay = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Explosion", meta = (UMin = 10, ClampMin = 10, Units = "cm"))
	float explosionRadius = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Explosion", meta = (UMin = 1, ClampMin = 1))
	int minExplosionDamage = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Explosion", meta = (UMin = 1, ClampMin = 1))
	int maxExplosionDamage = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Explosion")
	TArray<TEnumAsByte<EObjectTypeQuery>> explosionDamageLayer;

	bool canExploded = true;
	int damageDiff = 0.0f;

	float damageMultiplier = 1.0f;

	float bonusDamage;
	float bleedRate;
	FStatusParams bleedBonusParams;

public:
	FORCEINLINE void SetBonusDamage(const float _bonus) { bonusDamage = _bonus; }

	FORCEINLINE void SetBleedBonusParams(const float _value, const float _duration, const float _bleedRate)
	{
		bleedBonusParams.value = _value;
		bleedBonusParams.duration = _duration;
		bleedRate = _bleedRate;
	}

public:
	ANailgunProjectile();

	void SetNailgun(ANailGunActor* _nailgun);
	void Init(const float _damageMultiplier, ULifeComponent* _life = nullptr);

	TArray<FHitResult> MakeDamage();
	void Explode(const float _explosionRadius, const float _min, const float _max);
	void MakeExplosion();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UFUNCTION(Server, Unreliable) void ServerRpc_CallOnExplosionEvent();
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_CallOnExplosionEvent();

	UFUNCTION() void ActorDie();

	void AddBleed(AActor* _target);
};
