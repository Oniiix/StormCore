// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CalfActor.h"
#include "Components/CapsuleComponent.h"
#include "RobotHunter/Utils/ENUM_TriggerZone.h"
#include "EjectKickActor.generated.h"

#define EJECT_KICK_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AEjectKickActor : public ACalfActor
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReachKick,bool,_status);

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|BackDash|Jump")
		float height = 100;
	UPROPERTY(EditAnywhere, Category = "Custom|BackDash|Jump")
		float duration = 2;
	UPROPERTY(EditAnywhere, Category = "Custom|BackDash|Jump", meta = (UIMin = 0, ClampMin = 0, UIMax = 1, ClampMax = 1))
		float alpha = 0.5f;
		UPROPERTY(EditAnywhere, Category = "Custom|Kick")
		float range = 200;
		UPROPERTY(EditAnywhere, Category = "Custom|Kick")
		float radius = 50;
		UPROPERTY(EditAnywhere, Category = "Custom|Kick")
		TArray<TEnumAsByte<EObjectTypeQuery>> layer;
		UPROPERTY(EditAnywhere, Category = "Custom|Kick")
		float kickUpForce = 200;
		UPROPERTY(EditAnywhere, Category = "Custom|Kick")
		float kickForwardForce = 800;
		UPROPERTY(EditAnywhere, Category = "Custom|Kick")
		float kickTime = 2;
	UPROPERTY(EditAnywhere, Category = "Custom|BackDash")
		float backDashForce = 500;
	UPROPERTY(EditAnywhere, Category = "Custom|BackDash")
		float backDashUpForce = 500;
		bool canKick = true;
		bool isKicking = false;
		bool hasHit = false;
		UPROPERTY()
		FTimerHandle kickTimer;


		UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
		FOnReachKick onReachKick;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* hitComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Shape")
	float capsuleRadius = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Shape")
	float capsuleHeight = 44.0f;
	

public:
	AEjectKickActor();

	FORCEINLINE FOnReachKick& OnReachKick() { return onReachKick; }
	UFUNCTION(BlueprintCallable) FORCEINLINE  bool GetIsKicking() { return isKicking; }

public:	
	virtual void InitMember() override;
	virtual void PressBehavior();
	virtual void ReleaseBehavior() override;
	virtual bool CanUse() override;
	virtual void ResetMember() override;

	UFUNCTION() void Kick(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void SetupKick();
	UFUNCTION() void EndKick();
	UFUNCTION(BlueprintCallable) void EjectJump();
	virtual void StartCooldown() override;

	virtual void SetupMember() override;

private:
	UFUNCTION(Server, Unreliable) void ServerRpc_StartKick(const FRotator& _armRotation);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_StartKick(const FRotator& _armRotation);


	UFUNCTION(Server, Unreliable) void ServerRpc_Hit();
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_Hit();

	UFUNCTION(Server, Unreliable) void ServerRpc_Setup();
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_Setup();

	UFUNCTION() void UpdateRotation(FVector2D _direction, FRotator& _outRotation);
};
