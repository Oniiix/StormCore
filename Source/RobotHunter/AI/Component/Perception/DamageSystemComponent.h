// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerceptionSystemComponent.h"
#include "DamageSystemComponent.generated.h"

#define DAMAGE_COMPONENT_USE_LOG false
#define WEAK_POINT "Weak_Socket"

class ULifeComponent;
class UCapsuleComponent;

/**
 * The damage system of AI to detect player when it attack
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UDamageSystemComponent : public UPerceptionSystemComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamage);
	FOnTakeDamage onTakeDamage;

	TObjectPtr<UCapsuleComponent> weakPoint = nullptr;

public:
	UDamageSystemComponent();

	FORCEINLINE FOnTakeDamage& OnTakeDamage() { return onTakeDamage; }

	void InitPerception() override;

private:
	UFUNCTION()
	void TakeDamage(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);
};
