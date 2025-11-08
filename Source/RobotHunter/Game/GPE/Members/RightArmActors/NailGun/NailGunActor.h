// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RightArmActor.h"
#include <RobotHunter/Game/GPE/Projectile/NailGunProjectile/NailgunProjectile.h>
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NailGunActor.generated.h"

#define NAIL_GUN_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API ANailGunActor : public ARightArmActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeavyRecoil, bool, _isHeavy);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileGo, const FVector&, _startPosition, const FVector&, _direction);


	UPROPERTY(EditAnywhere, Category = "Custom|Nailgun|Hitscan")
		float rayLength = 10000;

	UPROPERTY(EditAnywhere, Category = "Custom|Nailgun|Hitscan")
		TArray<TEnumAsByte<EObjectTypeQuery>> layer;


	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
		FOnHeavyRecoil onHeavyRecoil;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnProjectileGo onProjectileGo;



	UPROPERTY(EditAnywhere, Category = "Custom|Nailgun|Projectile")
		TSubclassOf<ANailgunProjectile> nailRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Nailgun|Projectile")
		float minDistanceProjectileGo = 300.0f;

	TObjectPtr<UCameraComponent> playerCamera = nullptr;

	TObjectPtr<USpringArmComponent> springArm = nullptr;

public:
	FORCEINLINE FOnHeavyRecoil& OnHeavyRecoil() { return onHeavyRecoil; }
	FORCEINLINE FOnProjectileGo& OnProjectileGo() { return onProjectileGo; }
	
	FORCEINLINE TSubclassOf<ANailgunProjectile> GetNailgunProjectilRef() const { return nailRef; }

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Member_System
	virtual void InitMember() override;
	virtual void PressBehavior() override;
	virtual void ReleaseBehavior()override;
	virtual bool CanUse() override;
	virtual void ResetMember() override;

	//Recoil
	void Recoil();
	UFUNCTION(Client, WithValidation, UnReliable) void ClientRpc_Recoil();
	UFUNCTION(Server, UnReliable) void ServerRpc_Recoil();

	//Heavy_Recoil
	void HeavyRecoil();
	UFUNCTION(Client, WithValidation, UnReliable) void ClientRpc_HeavyRecoil();
	UFUNCTION(Server, UnReliable) void ServerRpc_HeavyRecoil();

	//ProjectileGo
	void CallProjectileGo(const FVector& _startPosition, const FVector& _direction);
	UFUNCTION(Client, WithValidation, UnReliable) void ClientRpc_CallProjectileGo(const FVector& _startPosition, const FVector& _direction);
	UFUNCTION(Server, UnReliable) void ServerRpc_CallProjectileGo(const FVector& _startPosition, const FVector& _direction);

	//Shoot
	void ScopeShoot();
	void UnscopeShoot();
	UFUNCTION(Server, UnReliable) void ServerRpc_SpawnNail(const FHitResult& _result);
};
