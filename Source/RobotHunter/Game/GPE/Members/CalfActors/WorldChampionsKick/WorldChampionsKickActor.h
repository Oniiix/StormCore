// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CalfActor.h"
#include "WorldChampionsKickActor.generated.h"

#define WORLD_CHAMPIONS_KICK_USE_LOG true

UCLASS(HideDropdown, HideCategories = ("Charge", "Camera", "HLOD"))
class ROBOTHUNTER_API AWorldChampionsKickActor : public ACalfActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWCKick, bool, _status);
	
	UPROPERTY(EditAnywhere, Category = "Custom|Property|WCK", meta = (UMin = 0, ClampMin = 0))
	float ejectionForce = 1;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|WCK")
	bool useRadians = false;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|WCK", meta = (UMin = 0, ClampMin = 0))
	float ejectionAngle = 45;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|WCK", meta = (UMin = 0, ClampMin = 0))
	float kickRange = 350;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|WCK", meta = (UMin = 0, ClampMin = 0, UMax = 360, ClampMax = 360))
	float kickAngle = 145;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|WCK", meta = (UMin = 0, ClampMin = 0))
	TArray<TEnumAsByte<EObjectTypeQuery>> kickLayers;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnWCKick onWCKick;

public:
	FORCEINLINE FOnWCKick& OnWCKick() { return onWCKick; }

public:


	virtual void PressBehavior() override;
	virtual bool CanUse() override;
	virtual void ResetMember() override;
	virtual void InitMember() override;

	UFUNCTION(NetMulticast, WithValidation, UnReliable) void ClientRpc_CallAnimation();
	UFUNCTION(Server,UnReliable) void ServerRpc_CallAnimation();

	virtual void SetupMember() override;

protected:
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;
#endif

private:
	UFUNCTION() void FinishWCK();

	void DrawDebug();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
