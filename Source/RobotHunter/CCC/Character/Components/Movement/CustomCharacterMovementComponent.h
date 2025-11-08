

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustomLanded, ACharacter*, _chara);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnCustomLanded onCustomLanded;

	UPROPERTY()
	TObjectPtr<class ACharacter> character = nullptr;

	float initGravityScale = 2.2f;
	float initFallingBrakingDeceleration = 1024.0f;
	float initWalkingBrakingDeceleration = 1024.0f;
	float initGroundFriction = 2.0f;

	float initMaxAcceleration = 4000.0f;

	bool onHandcar = false;

	TObjectPtr<class ACustomHandcarActor> handcar = nullptr;

public:
	FORCEINLINE FOnCustomLanded& OnCustomLanded() { return onCustomLanded; }

	FORCEINLINE bool GetOnHandcar() const { return onHandcar; };
	FORCEINLINE void SetOnHandcar(const bool _value, class ACustomHandcarActor* _handcar) { 
		onHandcar = _value;
		handcar = _handcar;
	};


	void SetVelocity(const FVector& _newVel, const float _gravity = 2.2f);
	void NetSetVelocity(const FVector& _newVel, const float _gravity = 2.2f);

	void SetMaxAcceleration(const float _newValue);
	void NetSetMaxAcceleration(const float _newValue);

	void LaunchCharacter(const FVector& _force, const float _gravity = 2.2f, const float _brakingDeceleration = 1024.0f, const bool _resetOnLanded = false);
	void NetLaunchCharacter(const FVector& _force, const float _gravity = 2.2f, const float _brakingDeceleration = 1024.0f, const bool _resetOnLanded = false);

	void SetFrictionValue(const float _groundFriction, const float _brakingDeceleration);
	void NetSetFrictionValue(const float _groundFriction, const float _brakingDeceleration);

	void ResetFriction();
	void NetResetFriction();

	void ResetMaxAcceleration();
	void NetResetMaxacceleration();

	void ResetGravityScale();
	void NetResetGravityScale();


	void InitMovement();
protected:
	virtual void BeginPlay() override;
	virtual void UpdateBasedMovement(float DeltaSeconds) override;

private:
	UFUNCTION(Server, UnReliable) void ServerRpc_SetVelocity(const FVector& _value, const float _gravity);
	UFUNCTION(NetMulticast, WithValidation, UnReliable) void ClientRpc_SetVelocity(const FVector& _value, const float _gravity);

	UFUNCTION(Server, UnReliable) void ServerRpc_SetMaxAcceleration(const float _value);
	UFUNCTION(NetMulticast, WithValidation, UnReliable) void ClientRpc_SetMaxAcceleration(const float _value);

	UFUNCTION(Server, UnReliable) void ServerRpc_LaunchCharacter(const FVector& _force, const float _gravity, const float _brakingDeceleration, const bool _resetOnLanded);


	UFUNCTION(Server, Reliable) void ServerRpc_SetFrictionValue(const float _groundFriction, const float _brakingDeceleration);
	UFUNCTION(NetMulticast, WithValidation, Reliable) void ClientRpc_SetFrictionValue(const float _groundFriction, const float _brakingDeceleration);

	UFUNCTION(Server, Reliable) void ServerRpc_SetGravityScale(const float _gravityScale);
	UFUNCTION(NetMulticast, WithValidation, Reliable) void ClientRpc_SetGravityScale(const float _gravityScale);

	UFUNCTION() void OnCharacterLanded(const FHitResult& _hit);
};
