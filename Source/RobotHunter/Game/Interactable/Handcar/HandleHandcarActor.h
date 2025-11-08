#pragma once

#include "CoreMinimal.h"
#include "../InteractableActor.h"
#include "../../GPE/Handcar/HandcarDataAsset.h"
#include "Components/BoxComponent.h"
#include "InputMappingContext.h"
#include "HandleHandcarActor.generated.h"

class ACustomHandcarActor;

UCLASS()
class ROBOTHUNTER_API AHandleHandcarActor : public AInteractableActor
{
	GENERATED_BODY()

#pragma region Interaction

#pragma region Trigger
	UPROPERTY(EditAnywhere, Category = "Interaction|Trigger")
	TObjectPtr<UBoxComponent> trigger;

	UPROPERTY()
	TObjectPtr<AMainCharacter> playerInTrigger;
#pragma endregion


	UPROPERTY(EditAnywhere, Category = "Interaction|Feedback")
	TObjectPtr<UTextRenderComponent> secondInteractionFeedback;


#pragma region Player
	UPROPERTY(EditAnywhere, Category = "Player|Location")
	TObjectPtr<USceneComponent> firstPlayerLocation;

	UPROPERTY(EditAnywhere, Category = "Player|Location")
	TObjectPtr<USceneComponent> secondPlayerLocation;


	//REPLIQUER
	UPROPERTY(ReplicatedUsing = OnRep_FirstPlayerUpdate)
	TObjectPtr<AMainCharacter> firstPlayer;

	//REPLIQUER
	UPROPERTY(ReplicatedUsing = OnRep_SecondPlayerUpdate)
	TObjectPtr<AMainCharacter> secondPlayer;


	//NE PAS REPLIQUER
	bool firstSideDetected;
	//NE PAS REPLIQUER
	bool secondSideDetected;
#pragma endregion

#pragma endregion


#pragma region Rotation
	//REPLIQUER
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> handleMesh;

	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float maxRotation;

	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float rotationValue;

	UPROPERTY(Replicated)
	float firstPlayerHeightRapport;
	UPROPERTY(Replicated)
	float secondPlayerHeightRapport;

	UPROPERTY(Replicated)
	float firstPlayerLastHeightRapport;
	UPROPERTY(Replicated)
	float secondPlayerLastHeightRapport;
#pragma endregion


#pragma region Handcar
	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;
#pragma endregion


	bool inputsSetup;

	TObjectPtr<class UNetworkGPEComponent> networkComponent = nullptr;

public:
	FORCEINLINE void SetHandcar(ACustomHandcarActor* _handcar) { handcar = _handcar; }

	FORCEINLINE void SetFirstPlayer(AMainCharacter* _chara) { firstPlayer = _chara; }
	FORCEINLINE void SetSecondPlayer(AMainCharacter* _chara) { secondPlayer = _chara; }

	FORCEINLINE TObjectPtr<ACustomHandcarActor> GetHandcar() const { return handcar; }

	FORCEINLINE TObjectPtr<AMainCharacter> GetFirstPlayer() const { return firstPlayer; }
	FORCEINLINE TObjectPtr<AMainCharacter> GetSecondPlayer() const { return secondPlayer; }

	FORCEINLINE float GetPlayerHeightRapport(const bool _isFirstPlayer) const { return _isFirstPlayer ? firstPlayerHeightRapport : secondPlayerHeightRapport; }
	FORCEINLINE float GetPlayerLastHeightRapport(const bool _isFirstPlayer) const { return _isFirstPlayer ? firstPlayerLastHeightRapport : secondPlayerLastHeightRapport; }

public:
	AHandleHandcarActor();


	void SetupPlayerInputs(AMainCharacter* _player);
private:


#pragma region Interaction

#pragma region Trigger
	UFUNCTION() void OnEnterTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION() void OnExitTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
#pragma endregion


#pragma region Acceleration
	void StartAcceleration(const FInputActionValue& _value);
	void ReleaseAcceleration(const FInputActionValue& _value);
#pragma endregion


	void UpdateHandleDetected(AMainCharacter* _player);
	void UpdateInteractionFeedbackVisibility();
#pragma endregion


#pragma region Rotation
	void RotateHandle(const float _maxRotation, const float _rotationValue, const float _deltaTime);
	void UpdateHandleHeightRapports(const bool _firstPlayerAccelerates, const bool _secondPlayerAccelerates);
#pragma endregion


	float GetClampPitch(const float _pitch);

protected:
	virtual void BeginPlay() override;
	virtual void TickUpdateInteractionFeedback() override;


public:
	virtual void Interacted(AMainCharacter* _character) override;

	void InHandcarTick(const bool _firstPlayerAccelerates, const bool _secondPlayerAccelerates, const float _deltaTime);

	void FirstPlayerInteraction(const FVector& _newPlayerLoc, AMainCharacter* _playerInteracting);
	void SecondPlayerInteraction(const FVector& _newPlayerLoc, AMainCharacter* _playerInteracting);

	void UpdateValuesFromDataAsset(UHandcarDataAsset* _da);

#pragma region Network
protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
private:
	UFUNCTION() void OnRep_FirstPlayerUpdate();
	UFUNCTION() void OnRep_SecondPlayerUpdate();
#pragma endregion
};
