// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "RobotHunter/Game/HitValue/HitValueActor.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "LifeComponent.generated.h"

class UDamageSystemComponent;

#define LIFE_COMPONENT_USE_LOG false

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Events
private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnDie onDie;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateLife, AActor*, _owner, const int, _currentLife, const int, _change, AActor*, _damageSource);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUpdateLife onUpdateLife;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLifeChangeByDamage, AActor*, _owner, const int, _currentLife, const int, _damage, AActor*, _damageSource);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLifeChangeByDamage onLifeChangeByDamage;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLifeChangeByHeal, AActor*, _owner, const int, _currentLife, const int, _heal, AActor*, _damageSource);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLifeChangeByHeal onLifeChangeByHeal;
#pragma endregion

#pragma region Properties
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_UpdateLife, Category = "Life_Property", BlueprintReadOnly, meta = (AllowPrivateAccess))
	int currentLife = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Life_Property")
	int maxLife = 100;

	UPROPERTY(EditAnywhere, Category = "Life_Property")
	int startLife = 100;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Life_Property", meta = (AllowPrivateAccess))
	int lastChangeSudden = 0;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Life_Property", meta = (AllowPrivateAccess))
	AActor* lastDamageSource = nullptr;

	UPROPERTY(EditAnywhere, Category = "Life_Property")
	TSubclassOf<AHitValueActor> hitActorReference = nullptr;
	UPROPERTY(EditAnywhere, Category = "Life_Property")
	bool canTakeDamage = true;

	bool shieldMode = false;

	float healBonus = 1.0f;
	float lifeStealBonus = 0.0f;

	bool isInit = false;

#pragma region Regen
private:
	float regenLife = 0.0f;
	FTimerHandle regenLifeTimer;
#pragma endregion

#pragma region Revive
private:
	bool canRevive = true;
	float reviveLifePercent = 0.0f;
#pragma endregion
private:
	float protection = 0.0f;
	float protectionBonus = 0.0f;
	float protectDirectionalAngle = 0.0f;
	float protectAngle = 360.0f;

	float counterBonus = 0.0f;

#pragma region Bleed
	float bleedRate;
	FStatusParams bleedBonusParams;
#pragma endregion

#pragma endregion
#pragma region Methods

public:
	ULifeComponent();

	// Event
	FORCEINLINE FOnDie& OnDie() { return onDie; }
	FORCEINLINE FOnUpdateLife& OnUpdateLife() { return onUpdateLife; }
	FORCEINLINE FOnLifeChangeByDamage& OnLifeChangeByDamage() { return onLifeChangeByDamage; }
	FORCEINLINE FOnLifeChangeByHeal& OnLifeChangeByHeal() { return onLifeChangeByHeal; }
	// Getter
	FORCEINLINE int GetCurrentLife() const { return currentLife; }
	//FORCEINLINE FVector GetHitPosition() const { return hitPosition; }
	FORCEINLINE int GetCurrentChange() const { return lastChangeSudden; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMaxLife() const { return maxLife; }

	FORCEINLINE bool GetShieldMode() const { return shieldMode; }
	FORCEINLINE bool GetCanRevive() const { return canRevive && reviveLifePercent > 0.0f; }

	// Setter
	FORCEINLINE void SetMaxLife(const int _maxLife) { maxLife = _maxLife; }
	FORCEINLINE void SetStartLife(const int _startLife) { 
		startLife = _startLife; 
		lastChangeSudden = startLife - currentLife;
		currentLife = startLife;
		if (isInit)
			OnRep_UpdateLife();
	}
	FORCEINLINE void SetShieldMode(const bool _shieldMode) { shieldMode = _shieldMode; }

	FORCEINLINE void SetHealBonus(const float _bonus) { healBonus = _bonus; }
	FORCEINLINE void SetLifeStealBonus(const float _bonus) { lifeStealBonus = _bonus; }

	FORCEINLINE void SetRegenLife(const float _regenLife)
	{
		regenLife = _regenLife;

		if (regenLife > 0.0f)
		{
			FTimerManager& _tm = GetWorld()->GetTimerManager();

			if (!_tm.IsTimerActive(regenLifeTimer))
				_tm.SetTimer(regenLifeTimer, this, &ULifeComponent::RegenLife, 1.0f, true);
		}
	}

	FORCEINLINE void SetReviveLifePercent(const float _reviveLifePercent) { reviveLifePercent = _reviveLifePercent; }

	FORCEINLINE void SetProtection(const float _protection) { protection = _protection; }
	FORCEINLINE void SetProtectionBonus(const float _bonus) { protectionBonus = _bonus; }
	FORCEINLINE void SetProtectDirectionalAngle(const float _protectDirectionalAngle) { protectDirectionalAngle = _protectDirectionalAngle; }

	FORCEINLINE void SetProtectAngle(const float _protectAngle) { protectAngle = _protectAngle; }

	FORCEINLINE void SetCounterBonus(const float _bonus) { counterBonus = _bonus; }
	
	FORCEINLINE void SetBleedBonusParams(const float _value, const float _duration, const float _bleedRate)
	{
		bleedBonusParams.value = _value;
		bleedBonusParams.duration = _duration;
		bleedRate = _bleedRate;
	}

	FORCEINLINE void EnableGodMode() { canTakeDamage = false; }
	FORCEINLINE void DisableGodMode() { canTakeDamage = true; }

	UFUNCTION()
	void SpawnHitValue(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);

	UFUNCTION(BlueprintCallable)
	void InitLife();

	UFUNCTION(BlueprintCallable)
	void Damage(const int _damage, AActor* _damageSource);

	UFUNCTION(BlueprintCallable)
	void RestoreLife(const int _heal);

	void SetLife(const int _currentLife, const int _change, AActor* _damageSource);

private:
	UFUNCTION()
	void OnRep_UpdateLife();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	bool CheckIfCanBeProtected(AActor* _damageSource);

	void LifeSteal();

	void RegenLife();
	void Revive();

	void Counter(const int _damage, AActor* _damageSource);
	void AddBleed(AActor* _actor);
#pragma endregion
};
