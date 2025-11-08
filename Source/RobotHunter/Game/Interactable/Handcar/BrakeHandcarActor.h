#pragma once

#include "CoreMinimal.h"
#include "../CameraInteractableActor.h"
#include "../../GPE/Handcar/HandcarDataAsset.h"
#include "InputMappingContext.h"
#include "BrakeHandcarActor.generated.h"

class ACustomHandcarActor;

UCLASS()
class ROBOTHUNTER_API ABrakeHandcarActor : public ACameraInteractableActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleaseBrake, bool, isReleased);

#pragma region Position
	UPROPERTY(EditAnywhere, Category = "Lever")
	TObjectPtr<USceneComponent> baseLever;

	UPROPERTY(EditAnywhere, Category = "Lever")
	TObjectPtr<UStaticMeshComponent> leverMesh;

#pragma region Angle
	UPROPERTY(EditAnywhere, Category = "Position|Angle", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float tightenedAngle;

	UPROPERTY(EditAnywhere, Category = "Position|Angle", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float releasedAngle;
#pragma endregion

#pragma region Speed
	//UPROPERTY(EditAnywhere, Category = "Position|Speed", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	//float minInteractionValue;

	//UPROPERTY(EditAnywhere, Category = "Position|Speed", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	//float maxInteractionValue;

	//UPROPERTY(EditAnywhere, Category = "Position|Speed", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	//float releasedSpeed;

	UPROPERTY(EditAnywhere, Category = "Position|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float speedPercent;
#pragma endregion

	//REPLIQUER OUI
	UPROPERTY(ReplicatedUsing=OnRep_UpdateIsReleased)
	bool isReleased;
	UPROPERTY(Replicated)
	float positionMultiplier;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Acceleration", meta = (UIMax = 0.0f, ClampMax = 0.0f))
	float brakeValue;

#pragma region Handcar
	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;

	float handcarMaxSpeed;
	float speedMultiplier;
	//float startingCoefficientFriction;
#pragma endregion
	
	//REPLIQUER
	UPROPERTY(Replicated)
	TObjectPtr<AMainCharacter> player;

	//bool inputsSetup;

	UPROPERTY()
	TObjectPtr<class UNetworkGPEComponent> networkComponent = nullptr;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnReleaseBrake onReleaseBrake;

public:
	FORCEINLINE void SetHandcar(ACustomHandcarActor* _handcar) { handcar = _handcar; }
	FORCEINLINE void SetHandcarMaxSpeed(const float _maxSpeed) { handcarMaxSpeed = _maxSpeed; }
	//FORCEINLINE void SetStartingCoefficientFriction(const float _coefficient) { startingCoefficientFriction = _coefficient; }

	FORCEINLINE void SetPlayer(AMainCharacter* _player) { player = _player; }
	FORCEINLINE void SetReleased(const bool _isReleased) { isReleased = _isReleased; onReleaseBrake.Broadcast(isReleased);}

	FORCEINLINE ACustomHandcarActor* GetHandcar() const { return handcar; }
	FORCEINLINE TObjectPtr<USceneComponent> GetBaseLever() const { return baseLever; }
	FORCEINLINE bool GetIsReleased() const { return isReleased; }
	FORCEINLINE float GetTightenedAngle() const { return tightenedAngle; }

	FORCEINLINE FOnReleaseBrake& GetOnReleaseBrake() { return onReleaseBrake; }

public:
	ABrakeHandcarActor();

	void TightenRotationLever(const FInputActionValue& _value);

private:
	//void SetupPlayerInputs(AMainCharacter* _player);

#pragma region Position
	//void ReleaseLever(const FInputActionValue& _value);

	//void ReleaseRotationLever(const float _deltaTime);

public:
	void InstantBrake(const bool _isReleased);

private:
	void UpdatePositionMultiplier();
	float GetPositionOnAngleScope();
#pragma endregion

	//void UpdateSpeedMultiplier(const float _handcarCurrentSpeed);
	//void UpdateHandcarCoefficientFriction();

protected:
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:
	virtual void Interacted(AMainCharacter* _character) override;

	//void InHandcarTick(const float _deltaTime, const float _handcarCurrentSpeed);
	float GetUpdatedBrakeValue(const float _handcarCurrentSpeed);

	void UpdateValuesFromDataAsset(UHandcarDataAsset* _da);

private:
	UFUNCTION() void OnRep_UpdateIsReleased();
};
