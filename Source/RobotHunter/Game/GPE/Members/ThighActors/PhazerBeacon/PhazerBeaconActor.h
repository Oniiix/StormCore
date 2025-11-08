// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "../ThighActors.h"
#include "BeaconActor.h"
#include "Camera/CameraComponent.h"
#include "PhazerBeaconActor.generated.h"

#define PHAZER_BEACON_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API APhazerBeaconActor : public AThighActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|Loading", meta = (UMin = 0.1f, ClampMin = 0.1f, Units = "Seconds"))
	float loadingTime = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|Loading", meta = (UMin = 0.1f, ClampMin = 0.1f, Units = "Seconds"))
	float maxLimitLoadingTime = 30.0f;

	//FTimerHandle loadingTimer;
	FTimerHandle maxLimitLoadingTimer;
	//UPROPERTY(VisibleAnywhere, Category = "Custom|PhazerBeacon|Loading")
	//bool loadingFinish = false;


	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|TeleportationSettings", meta = (UMin = 0, ClampMin = 0, Units = "Centimeters"))
	float teleportationRadius = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|TeleportationSettings", meta = (UMin = 0, ClampMin = 0, Units = "Centimeters"))
	float teleportationMaxDistance = 1500.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|TeleportationSettings")
	TArray<TEnumAsByte<EObjectTypeQuery>> lookLayers;
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|TeleportationSettings")
	TEnumAsByte<ECollisionChannel> correctLayer;
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|TeleportationSettings", meta = (UMin = 0, ClampMin = 0, Units = "Centimeters"))
	float wallOffset = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|PhazerBeacon|TeleportationSettings")
	TSubclassOf<ABeaconActor> beaconRef = nullptr;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = "OnRep_FirstBeaconUpdate", Category = "Custom|PhazerBeacon|TeleportationSettings")
	TObjectPtr<ABeaconActor> firstBeacon = nullptr;

	bool hasPressed = false;

	UPROPERTY()
	TObjectPtr<UCameraComponent> camera = nullptr;


public:
	APhazerBeaconActor();

	virtual void PressBehavior() override;
	virtual void ReleaseBehavior() override;

	void Teleport(const FVector& _location);

	virtual void InitMember() override;

protected:
	virtual void Tick(float _deltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	//void LoadingFinish();
	void MaxLimitLoadingFinish();
	void DrawPhazerDebug();

	UFUNCTION(Server, UnReliable) void ServerRpc_SpawnFirstBeacon(const FVector& _location);
	UFUNCTION() void OnRep_FirstBeaconUpdate();

	UFUNCTION(Server, UnReliable) void ServerRpc_Teleport(const FVector& _location);

};
