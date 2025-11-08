// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ThighActors.h"
#include "HaulerActor.generated.h"

#define HAULER_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AHaulerActor : public AThighActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseHauler);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUseHauler onUseHauler;
	
	UPROPERTY()
		bool canDash = true;
		bool hasStarted = false;

		UPROPERTY(EditAnywhere)
		TObjectPtr<UCurveFloat> speedCurve = nullptr;

	UPROPERTY(Replicated)
		FVector velocity;
	UPROPERTY(Replicated)
	FRotator rotation;

	UPROPERTY(EditAnywhere, meta = (Units = "Seconds"))
	float totalDashTime = 0;
	UPROPERTY(EditAnywhere, meta = (ForceUnits = "m/s"))
	float maxSpeed = 0;
	UPROPERTY(VisibleAnywhere, meta = (ForceUnits = "m"))
	float distance;
	UPROPERTY(EditAnywhere)
	float groundVelocityReduceMultiplier = 0.1;
	UPROPERTY(EditAnywhere)
	float fallVelocityReduceMultiplier = 0.1;

	UPROPERTY(EditAnywhere, meta = (UMin = 1, ClampMin = 1))
	int nbAirDash = 1;
	UPROPERTY(VisibleAnywhere)
	int currentNbAirDash = 0;

	float currentTime = 0;
	float currentSpeed = 0;
	FTimerHandle dashTimer;
	bool wasAiming = false;

public:
	FORCEINLINE FOnUseHauler& OnUseHauler() { return onUseHauler; }
	AHaulerActor();
	virtual void BeginPlay() override;
	virtual void InitMember() override;
	virtual void PressBehavior();
	virtual bool CanUse() override;
	void Dash();
	virtual void ReleaseBehavior();
	virtual void ResetMember() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, UnReliable) void ServerRpc_SetClientVelocity(FVector _velocity);
	UFUNCTION(NetMulticast, UnReliable) void MultiRpc_SetVelocity(FVector _velocity);
	UFUNCTION() void UpdateRotation(FVector2D _direction);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateSpeed();
	void EndDash();
	UFUNCTION() void StartDash();


private:
	bool CanDash();
	UFUNCTION() void TouchGround(const FHitResult& _result);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& _property) override;

#endif
};
