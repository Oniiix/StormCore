#pragma once

#include "CoreMinimal.h"
#include "../../../CustomActor.h"
#include "../../../Splines/Rail/RailActor.h"
#include "../../Interactable/Handcar/HandleHandcarActor.h"
#include "../../Interactable/Handcar/BrakeHandcarActor.h"
#include "../../Interactable/Handcar/NitroHandcarActor.h"
#include "RobotHunter/Game/Interactable/Handcar/LifeStockHandcarActor.h"
#include "RobotHunter/Game/Interactable/Handcar/EggStockHandcarActor.h"
#include "HandcarDataAsset.h"
#include "Curves/CurveFloat.h"
#include "Components/SphereComponent.h"
#include "CustomHandcarActor.generated.h"

class AStopHandcarActor;
class ARailsTargetActor;

UCLASS()
class ROBOTHUNTER_API ACustomHandcarActor : public ACustomActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStop, bool, isStopped, bool, inAutoMode);

#pragma region Data Asset
	UPROPERTY(EditAnywhere, Category = "Data Asset")
	bool useDataAsset;

	UPROPERTY(EditAnywhere, Category = "Data Asset")
	TObjectPtr<UHandcarDataAsset> dataAsset;
#pragma endregion


#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool frictionDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool brakeDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool slopeAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool slopeAngleDebug;

	/*UPROPERTY(EditAnywhere, Category = "Debug")
	bool pumpAccelerationDebug;*/

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool nitroAccelerationDebug;

	/*UPROPERTY(EditAnywhere, Category = "Debug")
	bool frictionSlopeAccelerationDebug;*/

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool totalAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool speedDebug;
#pragma endregion
protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAcess))
	TObjectPtr<UStaticMeshComponent> handcarMesh = nullptr;
private:
#pragma region Detection
	/*UPROPERTY(EditAnywhere, Category = "Rail|Detection", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float detectionRange;*/

	UPROPERTY(EditAnywhere, Category = "Handcar")
	TObjectPtr<UBoxComponent> hitBox;

	/*UPROPERTY()
	TArray<ARailActor*> detectedRails;*/
	UPROPERTY(VisibleAnywhere)
	TArray<AMainCharacter*> players;
#pragma endregion

#pragma region Bump
	UPROPERTY(EditAnywhere)
	TArray<UBoxComponent*> bumpBoxs;

	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0, Units="cm"))
	float bumpUpForce = 5000;
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0, Units= "cm"))
	float bumpForwardForce = 5000;
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0))
	float bumpGravity = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Bump", meta = (UMin = 0, ClampMin = 0, Units = "cm" ))
	float bumpDeceleration = 100.0f;
#pragma endregion


#pragma region Movement
	UPROPERTY(EditAnywhere, Category = "Rail")
	TObjectPtr<ARailActor> currentRail;


	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float rotationSpeed;


	bool canMove;
	bool isBlocked;
	bool isOnReturn;

	UPROPERTY(Replicated)
	float currentDistance;
#pragma endregion


#pragma region Handle
	/*UPROPERTY(EditAnywhere, Category = "Handle")
	TObjectPtr<AHandleHandcarActor> handle;*/
#pragma endregion


#pragma region Brake
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Brake", meta = (AllowPrivateAccess))
	TObjectPtr<ABrakeHandcarActor> brake;


	float brakeValue;
#pragma endregion


#pragma region Nitro
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Nitro", meta = (AllowPrivateAccess))
	TObjectPtr<ANitroHandcarActor> nitro;
#pragma endregion


#pragma region LifeStock
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LifeStock", meta = (AllowPrivateAccess))
	TArray<ALifeStockHandcarActor*> lifeStocks;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULifeComponent> lifeComponent;
#pragma endregion


#pragma region EggStock
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "EggStock", meta = (AllowPrivateAccess))
	TArray<AEggStockHandcarActor*> eggsStock;
#pragma endregion


#pragma region Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f, AllowPrivateAccess))
	float maxSpeed;


#pragma region Friction
	/*UPROPERTY(EditAnywhere, Category = "Speed | Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float coefficientFriction;*/

	UPROPERTY(EditAnywhere, Category = "Speed|Friction", meta = (UIMax = 0.0f, ClampMax = 0.0f))
	float frictionValue;

	/*UPROPERTY(EditAnywhere, Category = "Speed|Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float frictionChangeAngle;*/

	UPROPERTY(EditAnywhere, Category = "Speed|Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float frictionSpeedPercent;
#pragma endregion

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float currentSpeed;

	float maxRotSpeed;
#pragma endregion


#pragma region Acceleration
	/*UPROPERTY(EditAnywhere, Category = "Acceleration|Pump", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float pumpMinForce;

	UPROPERTY(EditAnywhere, Category = "Acceleration|Pump", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float pumpMaxForce;


	UPROPERTY(EditAnywhere, Category = "Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float accelerationBoost;*/

	UPROPERTY(EditAnywhere, Category = "Acceleration")
	TObjectPtr<UCurveFloat> slopeCurve;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	float currentAcceleration;
	//float frictionSlopeAcceleration;
	float slopeAcceleration;
	//float pumpAcceleration;

	//REPLIQUER
	//bool firstPlayerAccelerates;
	//REPLIQUER
	//bool secondPlayerAccelerates;
#pragma endregion


#pragma region Stop
	UPROPERTY(EditAnywhere, Category = "Stop")
	float stopTime;

	UPROPERTY(VisibleAnywhere, Category = "Stop")
	TMap<FString, AStopHandcarActor*> callingStops;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Stop")
	TObjectPtr<AStopHandcarActor> nextStop;

	UPROPERTY(EditAnywhere, Category = "Stop")
	TObjectPtr<AStopHandcarActor> currentStop;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStop onStop;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateInAutoMode)
	bool inAutoMode;
	bool isAtStop;
	bool isStopped;

	FTimerHandle stopTimer;

	UPROPERTY(ReplicatedUsing = OnRep_UpdatePosition)
	FVector position;
#pragma endregion

	UPROPERTY(VisibleAnywhere, Category = "Target")
	TArray<ARailsTargetActor*> currentTargets;

	UPROPERTY()
	TObjectPtr<UNetworkGPEComponent> networkComp;

public:
	//FORCEINLINE void SetCoefficientFriction(const float _coefficient) { coefficientFriction = _coefficient; }
	FORCEINLINE void SetPlayerAccelerates(const bool _isFirstPlayer, const bool _accelerates) 
	{
		/*if (_isFirstPlayer)
			firstPlayerAccelerates = _accelerates;
		else
			secondPlayerAccelerates = _accelerates;*/
	}
	FORCEINLINE void SetIsAtStop(const bool _isAtStop) { isAtStop = _isAtStop; }
	FORCEINLINE void SetCurrentStop(AStopHandcarActor* _stop) { currentStop = _stop; }

	FORCEINLINE bool GetInAutoMode() const { return inAutoMode; }
	FORCEINLINE bool GetIsOnReturn() const { return isOnReturn; }

	FORCEINLINE float GetCurrentSpeed() const { return currentSpeed; }

	FORCEINLINE ARailActor* GetCurrentRail() const { return currentRail; }
	FORCEINLINE ABrakeHandcarActor* GetBrakeActor() const { return brake; }
	FORCEINLINE ANitroHandcarActor* GetNitroActor() const { return nitro; }
	FORCEINLINE UStaticMeshComponent* GetHandcarMesh() const { return handcarMesh; }

	FORCEINLINE AStopHandcarActor* GetNextStop() const { return nextStop; }
	FORCEINLINE ARailsTargetActor* GetCurrentTarget() const
	{
		if (currentTargets.IsEmpty())
			return nullptr;

		const int _index = currentTargets.Num() - 1;
		return currentTargets[_index];
	}

	FORCEINLINE FOnStop& GetOnStop() { return onStop; }

	UFUNCTION(BlueprintCallable) FORCEINLINE float GetUpdatedNitroAcceleration() const
	{
		if (nitro)
			return nitro->GetUpdatedNitroAcceleration();

		return 0.0f;
	}
		 

public:
	ACustomHandcarActor();

	UFUNCTION(BlueprintCallable) void SetBumpBoxArray(const TArray<UBoxComponent*>& _boxArray);

	void PlayerOverlaps(AMainCharacter* _player);
private:
	//void UpdateDetectionSphere();
	void LogDebug();
	void GetDataAssetValues();

	void CheckIfStartAutoMode();
	void AutoMode();

	void UnlockLifeAndEggStocks(const bool _unlock);


	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


#pragma region Detection
	//UFUNCTION() void RefreshDetectedRails();

	//bool CheckIfRailIsOnPath(ARailActor* _rail);
	//bool CheckIfLeverContainsRail(ALeverActor* _lever, ARailActor* _rail);
	//bool CheckIfTargetContainsRail(ARailsTargetActor* _target, ARailActor* _rail);
#pragma endregion


#pragma region Movement
	void Move(const float _deltaTime);
	void Rotate(const float _deltaTime);

	void UpdateCurrentRail();
#pragma endregion


#pragma region Speed

#pragma region Acceleration
public:
	void FirstPlayerStartAcceleration();
	void FirstPlayerReleaseAcceleration();


	void SecondPlayerStartAcceleration();
	void SecondPlayerReleaseAcceleration();

private:
	//void UpdateCurrentAcceleration();
	void UpdateCurrentAcceleration(/*const float _lowestHeightRapport, const float _highestHeightRapport*/);


	//void UpdateFrictionSlopeAcceleration();
	void UpdateSlopeAcceleration();
	//void UpdatePumpAcceleration(const float _lastHeight, const float _currentHeight);
#pragma endregion


	void UpdateCurrentSpeed(const float _deltaTime);
#pragma endregion


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
	void AddCallingStop(AStopHandcarActor* _stop);
	void AddCallingStop_Internal(AStopHandcarActor* _stop);

	void RemoveCallingStop(AStopHandcarActor* _stop);
	void UpdateNextStop();

	void AddCurrentTarget(ARailsTargetActor* _target);
	void RemoveCurrentTarget(ARailsTargetActor* _target);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void RestoreLife(const float _life);

	void SetInAutoMode(const bool _inAutoMode);
	UFUNCTION() void OnRep_UpdateInAutoMode(const bool _inAutoMode);

private:
	UFUNCTION() void OnRep_UpdatePosition();

	void SetPosition();

	UFUNCTION() void OnBumpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
