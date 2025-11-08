// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "../LeftArmActor.h"
#include <NiagaraSystem.h>
#include <RobotHunter/Game/GPE/Projectile/CableurProjectile/CableurProjectile.h>
#include <RobotHunter/Game/GPE/Projectile/CableurProjectile/CableurNail.h>
#include "Camera/CameraComponent.h"
#include "CableurActor.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API ACableurActor : public ALeftArmActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float minDistance = 1;
	UPROPERTY(EditAnywhere)
	float maxDistance = 5;
	UPROPERTY(EditAnywhere)
	float rayLength = 1000;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> layer;

	FVector firstPosition;
	FVector secondPosition;

	bool hasFirstLocation = false;

	bool canUse = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACableurProjectile> cableProjectile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACableurNail> nailProjectile;

	UPROPERTY()
	TObjectPtr<ACableurNail> firstNail;
	UPROPERTY()
	TObjectPtr<ACableurNail> secondNail;

	UPROPERTY()
	TObjectPtr<UCameraComponent> camera = nullptr;
	

public:
	virtual void InitMember() override;
	virtual void PressBehavior()override;
	void InitCable(ACableurProjectile* _cable);
	virtual void ReleaseBehavior()override;
	
private:
	void SpawnFirstNail(const FVector& _position);
	void SpawnSecondNail(const FVector& _position);




	UFUNCTION(Server, Unreliable) void ServerRpc_SpawnFirstCable(const FVector& _position);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_SpawnFirstCable(const FVector& _position);
	UFUNCTION(Server, Unreliable) void ServerRpc_SpawnSecondCable(const FVector& _position);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_SpawnSecondCable(const FVector& _position);

};
