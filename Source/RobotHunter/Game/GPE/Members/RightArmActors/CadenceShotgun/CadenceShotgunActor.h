// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "../RightArmActor.h"
#include "RobotHunter/UI/HUD/GameHUD.h"
#include "Camera/CameraComponent.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "CadenceShotgunActor.generated.h"

#define CADENCE_SHOTGUN_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API ACadenceShotgunActor : public ARightArmActor
{
	GENERATED_BODY()
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCadenceShoot, const FVector&, _startPosition, const FVector&, _shootDirection, const int, _lineTraceNumber);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumberOfStacksChange, int, _stacks);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReductionUpdate, float, _reductionRatio);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailed);

	//UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	//FOnCadenceShoot onCadenceShoot;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnNumberOfStacksChange onNumberOfStacksChange;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnReductionUpdate onReductionUpdate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnFailed onFailed;


	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Rhythm", meta = (UMin = 0.1, ClampMin = 0.1, Units = "Seconds"))
	float reductionDuration = 5;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Rhythm", meta = (UMin = 0, ClampMin = 0, UMax = 100, ClampMax = 100, Units = "Percent"))
	float rhythmMeasureMin = 40;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Rhythm", meta = (UMin = 0, ClampMin = 0, UMax = 100, ClampMax = 100, Units = "Percent"))
	float rhythmMeasureMax = 60;

	UPROPERTY(VisibleAnywhere, Category = "Custom|CadenceShotgun|Rhythm", meta = (Units = "Seconds"))
	float bonusTime = 1;

	float currentTime = 0;


	UPROPERTY(VisibleAnywhere, Category = "Custom|CadenceShotgun|Stacks")
	int stacks = 0;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Stacks", meta = (Units = "Seconds"))
	float expirationTime = 11;

	FTimerHandle expirationTimer;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Failed", meta = (Units = "Seconds"))
	float failedCooldown = 1;


	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Shoot", meta = (Units = "Centimeters"))
	float shootRange = 500;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Shoot", meta = (ForceUnits = "Radius"))
	float shootCircleRadius = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shotgun|Shoot", meta = (UMin = 1, ClampMin = 1, DisplayName = "Number of Shots", AllowPrivateAccess))
	float nbOfShot = 8;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun|Shoot")
	TArray<TEnumAsByte<EObjectTypeQuery>> shootLayers;

	UPROPERTY(EditAnywhere, Category = "Custom|CadenceShotgun")
	FStatusParams cooldownModifierStatus = FStatusParams(EStatusType::CD_MODIF, -1.0f, 10.0f, 3.0f);

	UPROPERTY()
	TObjectPtr<AGameHUD> gameHUD = nullptr;
	UPROPERTY()
	TObjectPtr<UCameraComponent> camera = nullptr;
	UPROPERTY()
	TObjectPtr<UStatusComponent> statusComp = nullptr;

public:
#pragma region Getter
	FORCEINLINE FOnNumberOfStacksChange& OnNumberOfStacksChange() { return onNumberOfStacksChange; }
	FORCEINLINE FOnReductionUpdate& OnReductionUpdate() { return onReductionUpdate; }
	FORCEINLINE FOnFailed& OnFailed() { return onFailed; }

	FORCEINLINE float GetCurrentTime() const { return currentTime; }
	FORCEINLINE float GetReductionDuration() const { return reductionDuration; }
	FORCEINLINE float GetReductionRatio() const { return FMath::Clamp(currentTime / reductionDuration, 0.0f, 1.0f); }
	FORCEINLINE float GetRhythmMeasureMin() const { return rhythmMeasureMin; }
	FORCEINLINE float GetRhythmMeasureMax() const { return rhythmMeasureMax; }
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetStacks(const int _stacks) 
	{
		const float _maxStack = cooldownModifierStatus.maxStack;
		stacks = _stacks > _maxStack ? _maxStack : _stacks;
		onNumberOfStacksChange.Broadcast(stacks);
	}
#pragma endregion

	virtual void PressBehavior() override;
	virtual void InitMember() override;

protected:
	virtual void Tick(float _deltaTime) override;


private:
	void ExpirationFinish();
	void Shoot();
	void DrawDebug();

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UFUNCTION(Server, Unreliable) void ServerRpc_CallonFailedEvent();
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_CallonFailedEvent();
};
