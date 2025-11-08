#pragma once

#include "CoreMinimal.h"
#include "../WagonActor.h"
#include "InputMappingContext.h"
#include "HandcarActor.generated.h"

#define HANDCAR_USE_LOG false

UCLASS()
class ROBOTHUNTER_API AHandcarActor : public AWagonActor
{
	GENERATED_BODY()
	
#pragma region Inputs
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> handcarContext;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> gameContext;

	bool serverInputsSetup;
	bool clientInputsSetup;
#pragma endregion

#pragma region Spawn
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TObjectPtr<USceneComponent> firstPlayerSpawnLocation;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TObjectPtr<USceneComponent> secondPlayerSpawnLocation;
#pragma endregion Spawn

#pragma region Interaction
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TObjectPtr<UTextRenderComponent> secondInteractionFeedback;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> triggerZone = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float inputTime;

	UPROPERTY(EditAnywhere, Category = "Interaction | Horn")
	float hornCooldown;

	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<AMainCharacter> firstPlayer;

	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<AMainCharacter> secondPlayer;

	FTimerHandle inputTimer;
	FTimerHandle hornTimer;
#pragma endregion Interaction

#pragma region Handle
	UPROPERTY(EditAnywhere, Category = "Handle | Mesh")
	TObjectPtr<UStaticMeshComponent> handleMesh;

	UPROPERTY(EditAnywhere, Category = "Handle")
	TObjectPtr<USceneComponent> firstHandlePlayerLocation;

	UPROPERTY(EditAnywhere, Category = "Handle")
	TObjectPtr<USceneComponent> secondHandlePlayerLocation;

	UPROPERTY(EditAnywhere, Category = "Handle")
	float maxHandleRotation;

	UPROPERTY(EditAnywhere, Category = "Handle")
	float handleRotationSpeed;

	UPROPERTY(VisibleAnywhere)
	bool firstHandleDetected;
	UPROPERTY(VisibleAnywhere)
	bool secondHandleDetected;

	float firstPlayerHandleHeightRapport;
	float secondPlayerHandleHeightRapport;

	float accelerationStartingHeight;
#pragma endregion Handle

#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Speed")
	float normalSpeed;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float boostedSpeed;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float accelerationForce;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float decelerationForce;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float brakeDecelerationForce;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool firstPlayerAccelerates;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool secondPlayerAccelerates;

	UPROPERTY(Replicated)
	bool changeWayDeceleration;

	float accelerationStartingSpeed;
	float finalSpeed;

	FTimerHandle naturalDecelerationTimer;

	UPROPERTY(Replicated)
	FTimerHandle brakeDecelerationTimer;
#pragma endregion Speed

public:
	FORCEINLINE void SetFirstPlayer(AMainCharacter* _player) { firstPlayer = _player; }
	FORCEINLINE void SetSecondPlayer(AMainCharacter* _player) { secondPlayer = _player; }

	FORCEINLINE void SetFirstPlayerAccelerates(const bool _accelerates) { firstPlayerAccelerates = _accelerates; }
	FORCEINLINE void SetSecondPlayerAccelerates(const bool _accelerates) { secondPlayerAccelerates = _accelerates; }

public:
	AHandcarActor();

private:
#pragma region Inputs
	void SetupPlayerInputs(AMainCharacter* _player);
	void ChangeContext(AMainCharacter* _player, UInputMappingContext* _toDisable, UInputMappingContext* _toEnable);
#pragma endregion Inputs

#pragma region Interaction

public:
	void EnterHandcar(AMainCharacter* _player, const FVector& _handleLoc);
	void ExitHandcar(AMainCharacter* _player, const FVector& _spawnLoc);

	void FirstHandleInteraction(AMainCharacter* _chara);
	void SecondHandleInteraction(AMainCharacter* _chara);

	void ChangeWay(const FInputActionValue& _value);
	void ClientChangeWay(const FInputActionValue& _value);
	void Horn(const FInputActionValue& _value);

private:
	void NoInputForTooMuchTime();
#pragma endregion Interaction

#pragma region Handle
	void UpdateHandleDetected(AMainCharacter* _chara);
	void UpdateInteractionFeedbackVisibility();

	void RotateHandle(const float _maxHandleRotation, const float _handleRotationSpeed);
	void UpdateHandleHeightRapports();
#pragma endregion Handle

#pragma region Speed
	void StartAcceleration(bool& _playerAccelerates, const float _handleHeightRapport);
	void ReleaseAcceleration(bool& _playerAccelerates);

public:
	UFUNCTION(CallInEditor) void FirstPlayerStartAcceleration();
	UFUNCTION(CallInEditor) void FirstPlayerReleaseAcceleration();

	UFUNCTION(CallInEditor) void SecondPlayerStartAcceleration();
	UFUNCTION(CallInEditor) void SecondPlayerReleaseAcceleration();

private:
	void ServerStartAcceleration(const FInputActionValue& _value);
	void ServerReleaseAcceleration(const FInputActionValue& _value);
	void ClientStartAcceleration(const FInputActionValue& _value);
	void ClientReleaseAcceleration(const FInputActionValue& _value);

public:
	void SetBrakeDecelerationTimer();
	void ClearBrakeDecelerationTimer();

private:
	void StartBrakeDeceleration(const FInputActionValue& _value);
	void ReleaseBrakeDeceleration(const FInputActionValue& _value);

	void ClientStartBrakeDeceleration(const FInputActionValue& _value);
	void ClientReleaseBrakeDeceleration(const FInputActionValue& _value);

	void UpdateFinalSpeed();
	void UpdateFinalSpeedFromHandleHeightRapports(const float _lowestHeightRapport, const float _highestHeightRapport);

	void UpdateMovementSpeed();

	void Accelerates(const float _startingHeight, const float _currentHeight, const float _startingSpeed);

	void Decelerates(const float _decelerationForce);
	void NaturalDeceleration();
	void BrakeDeceleration();

#pragma endregion Speed

protected:
	virtual void BeginPlay() override;
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION() void OnEnterTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION() void OnExitTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	virtual void Tick(float DeltaTime) override;

	virtual void TickUpdateInteractionFeedback() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:
	virtual void Interacted(AMainCharacter* _chara) override;
};
