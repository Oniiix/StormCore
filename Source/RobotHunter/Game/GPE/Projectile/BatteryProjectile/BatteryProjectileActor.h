#pragma once

#include "CoreMinimal.h"
#include "../ProjectileActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Components/BoxComponent.h"
#include "BatteryProjectileActor.generated.h"

#define BATTERY_PROJECTILE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API ABatteryProjectileActor : public AProjectileActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunch);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, const FHitResult&, _results);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCanActivate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnAOE);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLaunch onLaunch;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnHit onHit;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnSpawnAOE onSpawnAOE;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float lifeTime;

	UPROPERTY(EditAnywhere, Category = "Parameters", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float cooldown;

	UPROPERTY(EditAnywhere, Category = "Parameters", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float radiusRange;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	TArray<TEnumAsByte<EObjectTypeQuery>> activateLayer;
	
	float weakMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> triggerBox = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> spearMesh = nullptr;
	UPROPERTY()
	TObjectPtr<AActor> parentActor = nullptr;
	

	bool isActive;
	FOnCanActivate onCanActivate;

public:
	ABatteryProjectileActor();
	FORCEINLINE float GetBatteryLifeTime() const { return lifeTime; }
	FORCEINLINE float GetBatteryCurrentLifeSpan()
	{
		float _result = GetLifeSpan() > 0 ? GetLifeSpan() : GetBatteryLifeTime();
		return _result;
	}
	FORCEINLINE FOnHit& OnHit() { return onHit; }
	FORCEINLINE FOnLaunch& OnLaunch() { return onLaunch; }
	FORCEINLINE FOnSpawnAOE& OnSpawnAOE() { return onSpawnAOE; }


	virtual void InitProjectile(AMainCharacter* _mainCharacter, const float _chargeTime = 0.0f, const float _maxChargeTime = 0.0f, const FVector _charaVelocity = FVector(0.0f)) override;

private:
	UFUNCTION() void Activate();
	void CheckForObjects(const TArray<FHitResult>& _result);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//UFUNCTION() void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void StopIfHit() override;

private:
	void CallOnHit(const FHitResult& _result);
	UFUNCTION(Server, Unreliable) void ServerRpc_CallOnHitEvent(const FHitResult& _result);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_CallOnHitEvent(const FHitResult& _result);

	
};
