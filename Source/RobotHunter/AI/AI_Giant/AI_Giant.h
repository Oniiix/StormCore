// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/AI_Base.h"
#include <NavigationSystem.h>
#include "AI_Giant.generated.h"

#define AI_TITAN_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AAI_Giant : public AAI_Base
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnableShield);
	FOnEnableShield onEnableShield;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableShield);
	FOnDisableShield onDisableShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Titan_Property", meta = (AllowPrivateAccess))
	TObjectPtr<AActor> playerFront = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Titan_Property", meta = (Umin = 0.1f, ClampMin = 0.1f, UMax = 1.f, ClampMax = 1.f))
	float frontLocationModifier = 0.75f;
	UPROPERTY(VisibleAnywhere, Category = "Titan_Property")
	TArray<TEnumAsByte<EObjectTypeQuery>> frontLayers;

	UPROPERTY(VisibleAnywhere, Category = "Titan_Property", meta = (Umin = 1, ClampMin = 1))
	int damageThreshold = 200;
	UPROPERTY(VisibleAnywhere, Category = "Titan_Property", meta = (Umin = 0.1f, ClampMin = 0.1f))
	float delayThreshold = 0.5f;
	UPROPERTY(VisibleAnywhere, Category = "Titan_Property", meta = (Umin = 0, ClampMin = 0))
	int shieldSpeed = 300;
	UPROPERTY(VisibleAnywhere, Category = "Titan_Property", meta = (Umin = 0.1f, ClampMin = 0.1f))
	float shieldDisableDelay = 3.f;
	UPROPERTY(EditAnywhere, Category = "Titan_Property")
	FStatusParams protectStatus = FStatusParams(EStatusType::PROTECT, 25.0f, -1.0f, 0.0f, 180.0f);

	bool usingShield = false;
	bool isStunning = false;
	bool inAttack = false;
	bool startThreshold = false;
	int currentDamage = 0;
	FTimerHandle timerShieldDelay;

	TObjectPtr<UNavigationSystemV1> navigationSystem = nullptr;
	TObjectPtr<AMainCharacter> player = nullptr;
	TObjectPtr<UCameraComponent> playerCamera = nullptr;

	bool isInitializationGood = false;

public:
	AAI_Giant();

	FORCEINLINE FOnEnableShield& OnEnableShield() { return onEnableShield; }
	FORCEINLINE FOnDisableShield& OnDisableShield() { return onDisableShield; }
	FORCEINLINE AActor* GetPlayerFront() const { return playerFront; }

	FORCEINLINE void SetFrontLayers(const TArray<TEnumAsByte<EObjectTypeQuery>>& _layers) { frontLayers = _layers; }

protected:
	void InitAI() override;
	void SetAI(AArena* _arena) override;

	FUniqueNetIdRepl StartToFight() override;
	void StunAI(const float _stunTime);

	void Tick(float DeltaTime) override;
	FVector UpdatePlayerFront();

	UFUNCTION()
	void CheckTitanDamage(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);
	void SlowAI(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource) override;
	UFUNCTION()
	void EndDamageThreshold();
	UFUNCTION()
	void EndStun();

	UFUNCTION()
	void StartAttack();   
	UFUNCTION()
	void EndAttack();

	void StartShield();
	UFUNCTION()
	void StopShield();
};
