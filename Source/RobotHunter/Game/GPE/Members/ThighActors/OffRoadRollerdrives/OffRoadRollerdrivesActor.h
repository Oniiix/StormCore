// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ThighActors.h"
#include "RobotHunter/Utils/ENUM_Speed.h"
#include "OffRoadRollerdrivesActor.generated.h"

#define OFFROAD_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AOffRoadRollerdrivesActor : public AThighActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseStamina, const float, _currentStamina, const float, _maxStamina);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUseStamina onUseStamina;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndStaminaReload);

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnEndStaminaReload onEndStaminaReload;

#pragma region Stamina
	UPROPERTY(EditAnywhere, Category = "Custom|RoadRollerdrives|Stamina")
	float maxStamina = 100;

	UPROPERTY(VisibleAnywhere, Category = "Custom|RoadRollerdrives|Stamina")
	float currentStamina = 100;

	UPROPERTY(EditAnywhere, Category = "Custom|RoadRollerdrives|Stamina",
		meta = (UMin = 0, ClampMin = 0, UMax = 100, ClampMax = 100, Units="Percent"))
	float exhaustionPercent = 50;

	float oldStamina = 0;
#pragma endregion

#pragma region Press
	UPROPERTY(EditAnywhere, Category = "Custom|RoadRollerdrives|Decrease")
	float decreaseValue = 5;
	UPROPERTY(EditAnywhere, Category = "Custom|RoadRollerdrives|Decrease", meta = (Units = "Seconds"))
	float decreaseTime = 1;

	UPROPERTY(VisibleAnywhere, Category = "Custom|RoadRollerdrives|Decrease")
	bool isRunning = false;

	float maxDecreaseTime = 10;
	//float fullDecreaseTime = 10;
	FTimerHandle speedTimer;
#pragma endregion

#pragma region Release
	UPROPERTY(EditAnywhere, Category = "Custom|RoadRollerdrives|Reload")
	float reloadingValue = 5;
	UPROPERTY(EditAnywhere, Category = "Custom|RoadRollerdrives|Reload", meta = (Units = "Seconds"))
	float reloadingTime = 1;

	UPROPERTY(VisibleAnywhere, Category = "Custom|RoadRollerdrives|Reload")
	bool isReloading = false;


	float maxReloadingTime = 10;
	//float fullReloadingTime = 10;
	FTimerHandle reloadingTimer;
#pragma endregion

#pragma region Acceleration
	UPROPERTY(EditAnywhere, Category="Custom|RoadRollerdrives|Acceleration", meta = (UMin = 0, ClampMin = 0))
	float defaultAcceleration = 6000.0f;
	UPROPERTY(EditAnywhere, Category="Custom|RoadRollerdrives|Acceleration", meta = (UMin = 0, ClampMin = 0))
	float startAcceleration = 8000.0f;
	UPROPERTY(EditAnywhere, Category="Custom|RoadRollerdrives|Acceleration",
		meta = (UMin = 0, ClampMin = 0, Units = "s"))
	float startAccelerationTime = 1.0f;

	FTimerHandle accelerationTimer;
#pragma endregion

public:
	/**
	 * Get the current of stamina (need to cooldown start in UI)
	 * @return currentStarmina value
	 */
	FORCEINLINE float GetCurrentStamina() const { return currentStamina; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsRunning() const { return isRunning; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsReloading() const { return isReloading; }

	FORCEINLINE FOnUseStamina& OnUseStamina() { return onUseStamina; }
	FORCEINLINE FOnEndStaminaReload& OnEndStaminaReload() { return onEndStaminaReload; }


	virtual void PressBehavior() override;
	virtual void ReleaseBehavior() override;
	virtual void StartCooldown() override;
	virtual void EndCooldown() override;
	virtual void ModifyCooldownMultiplier(const float _cooldownReducPercent) override;
	virtual void ModifyCooldownBonus(const float _cooldownReducPercent) override;
	UFUNCTION() virtual void ResetMember() override;
	virtual bool CanUse() override;

private:
	virtual void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void UseStamina();
	void FinishDecrease();

	void StartReloading();
	void FinishReloading();

	void SetSpeedMode(ESpeedMode _speedMode);

	UFUNCTION()
	void OnLanded(const FHitResult& _result);

	void StartReloadingTimerAfterModification(const float _oldMaxReloadingTime);

	UFUNCTION() void SwitchSprintState(const bool _status = true);
};
