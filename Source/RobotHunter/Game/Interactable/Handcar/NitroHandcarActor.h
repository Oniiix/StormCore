#pragma once

#include "CoreMinimal.h"
#include "../CameraInteractableActor.h"
#include "InputMappingContext.h"
#include "../../../UI/ATH/Draisine/DraisineATH.h"
#include "../../GPE/Handcar/HandcarDataAsset.h"
#include "NitroHandcarActor.generated.h"

class ACustomHandcarActor;

UCLASS()
class ROBOTHUNTER_API ANitroHandcarActor : public ACameraInteractableActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseControls, bool, isUsing);

	//REPLIQUER?
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> valveMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> driverSeat;

	UPROPERTY(EditAnywhere, Category = "Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroAcceleration;

	UPROPERTY()
	TObjectPtr<class UNetworkGPEComponent> networkComponent = nullptr;


	/*UPROPERTY(EditAnywhere, Category = "Cursor", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float cursorSpeed;*/

#pragma region Zone
	/*UPROPERTY(EditAnywhere, Category = "Zone", meta = (UIMin = 0.0f, UIMax = 100.0f, ClampMin = 0.0f, ClampMax = 100.0f))
	float greenZoneSize;

	UPROPERTY(EditAnywhere, Category = "Zone", meta = (UIMin = 0.0f, UIMax = 100.0f, ClampMin = 0.0f, ClampMax = 100.0f))
	float yellowZoneSize;*/
#pragma endregion

#pragma region Movement

#pragma region Angle
	/*UPROPERTY(EditAnywhere, Category = "Movement|Angle", meta = (UIMin = -90.0f, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float minMovementAngle;

	UPROPERTY(EditAnywhere, Category = "Movement|Angle", meta = (UIMin = -90.0, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float maxMovementAngle;

	float greenInitialAngularOffset;
	float yellowInitialAngularOffset;*/
#pragma endregion

#pragma region Speed
	/*UPROPERTY(EditAnywhere, Category = "Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minMovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxMovementSpeed;*/
#pragma endregion

#pragma region Time
	/*UPROPERTY(EditAnywhere, Category = "Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minMovementTime;

	UPROPERTY(EditAnywhere, Category = "Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxMovementTime;*/
#pragma endregion

#pragma endregion

#pragma region LifeConsumption
	/*UPROPERTY(EditAnywhere, Category = "LifeConsumption")
	float greenLifeConsumption;

	UPROPERTY(EditAnywhere, Category = "LifeConsumption")
	float yellowLifeConsumption;

	UPROPERTY(EditAnywhere, Category = "LifeConsumption")
	float redLifeConsumption;*/
#pragma endregion

#pragma region Handcar
	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;
#pragma endregion

	//REPLIQUER
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdatePlayer, meta = (AllowprivateAccess))
	TObjectPtr<AMainCharacter> player;

	/*UPROPERTY()
	TObjectPtr<UDraisineATH> ath;*/

	//REPLIQUER
	//bool nitroIsActive;
	//bool slidersAreMoving;
	bool inputsSetup;
	//bool isInitialized;

	//float currentSpeed;
	//float targetAngle;
	//REPLIQUER
	//float currentSliderAngle;
	//REPLIQUER
	//float currentCursorAngle;
	//REPLIQUER
	//float currentLifeConsumption;

	bool canChangeHandcarDirection;
	bool useNegativeNitro;

	UPROPERTY(Replicated)
	float updatedNitroAcceleration;

	//FTimerHandle movementTimer;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUseControls onUseControls;
public:
	FORCEINLINE void SetInputsSetup(const bool _isSetup) { inputsSetup = _isSetup; }

	FORCEINLINE void SetHandcar(ACustomHandcarActor* _handcar) { handcar = _handcar; }
	FORCEINLINE void SetNitroAcceleration(const float _nitroAcceleration) { nitroAcceleration = _nitroAcceleration; }
	FORCEINLINE void SetUseNegativeNitro(const bool _useNegative) { useNegativeNitro = _useNegative; }
	FORCEINLINE void SetUpdatedNitroAcceleration(const float _updatedNitroAcceleration) { updatedNitroAcceleration = _updatedNitroAcceleration; }
	FORCEINLINE void SetNitroPlayer(const TObjectPtr<AMainCharacter>& _character) { player = _character; }
	FORCEINLINE TObjectPtr<AMainCharacter> GetNitroPlayer() const { return player; }
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetDriverSeat() const { return driverSeat; }

	//FORCEINLINE bool GetNitroIsActive() const { return nitroIsActive; }
	//FORCEINLINE float GetNitroAcceleration() const { return nitroAcceleration; }
	//FORCEINLINE float GetCurrentLifeConsumption() const { return currentLifeConsumption; }
	UFUNCTION(BlueprintCallable)FORCEINLINE bool IsPlayerNull() { return player == nullptr; }

	FORCEINLINE FOnUseControls& GetOnUseControls() { return onUseControls; }
	UFUNCTION(BlueprintCallable)FORCEINLINE ACustomHandcarActor* GetHandcar() const { return handcar; }

public:
	ANitroHandcarActor();

	void SetupPlayerInputs(AMainCharacter* _player);

	UFUNCTION() void PauseHandcar(UInputMappingContext* _current);

private:
	//void GetATH();

	//void UpdateCursorColor();
	//bool CheckCursorIsInZone(const float _cursorValue, const ESliderColor& _color);

	//void InitializeSliders();
	//void StartMoveSliders();
	//void MoveSliders(const float _deltaTime);

	//void MoveCursor(const float _inputValue);
	//void MoveRightCursor(const FInputActionValue& _value);
	//void MoveLeftCursor(const FInputActionValue& _value);

	void UpdateNitroAcceleration(const FInputActionValue& _value);
	void ChangeHandcarDirection(const FInputActionValue& _value);
	void ResetCanChangeHandcarDirection(const FInputActionValue& _value);

	UFUNCTION() void OnRep_UpdatePlayer(AMainCharacter* _oldCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
public:
	virtual void Interacted(AMainCharacter* _character) override;

	//void InHandcarTick(const float _deltaTime);

	void UpdateValuesFromDataAsset(UHandcarDataAsset* _da);
	float GetUpdatedNitroAcceleration() const;
};
