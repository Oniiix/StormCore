// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AttackCQCDataAsset.generated.h"

#define USE_LOG_ATTACK_CQC false

class AAI_Base;
class AMainCharacter;

UENUM()
enum ECQCEffect
{
	Nothing,
	PushTarget,
	PushOwner
};

/**
 * DataAsset that store properties and method of CQC attack
 */
UCLASS(BlueprintType)
class ROBOTHUNTER_API UAttackCQCDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0))
	float damage = 10.f;
	UPROPERTY(EditAnywhere)
	FCollisionProfileName collisionProfile;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECQCEffect> effect = ECQCEffect::Nothing;
	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0, EditCondition = "effect != ECQCEffect::Nothing", EditConditionHides))
	int pushDistance = 1000;
	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0, UMax = 1, ClampMax = 1, EditCondition = "effect != ECQCEffect::Nothing", EditConditionHides))
	float pushSnapPower = 0.75f;
	UPROPERTY(EditAnywhere)
	bool drawDebug = false;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EAIAnimation> animationToLaunch = EAIAnimation::Null;

public:
	FORCEINLINE float GetDamage() const { return damage; }
	FORCEINLINE TEnumAsByte<ECQCEffect> GetEffect() const { return effect; }
	FORCEINLINE FCollisionProfileName GetCollisionProfile() const { return collisionProfile; }
	FORCEINLINE TEnumAsByte<EAIAnimation> GetAnimationToLaunch() const { return animationToLaunch; }

	/// <summary>
	/// Apply an effect (or not) on the attack
	/// </summary>
	/// <param name="_target"> the target attack </param>
	/// <param name="_owner"> the owner of the attack </param>
	void CQCEffect(AActor* _target, AActor* _owner);
private:
	void PushTarget(AActor* _target, AActor* _owner);
	void PushOwner(AActor* _target, AActor* _owner);
};
