#pragma once

#include "CoreMinimal.h"
#include "../../../CustomActor.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "ProjectileActor.generated.h"

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API AProjectileActor : public AActor
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = "Parameters|Debug")
	bool useVelocityDebug;

	UPROPERTY(EditAnywhere, Category = "Parameters|Damage")
	float damage;

	UPROPERTY(EditAnywhere, Category = "Parameters|Charge")
	bool useCharge;


#pragma region No charge
	UPROPERTY(EditAnywhere, Category = "Parameters|Force", meta = (EditCondition = "!useCharge", EditConditionHides))
	float initialZVelocityBoost;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed", meta = (EditCondition = "!useCharge", EditConditionHides))
	float initialSpeed;


#pragma region Resistance
	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "!useCharge", EditConditionHides))
	float distanceSwitch;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "!useCharge", EditConditionHides))
	float initialResistance;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "!useCharge", EditConditionHides))
	float finalResistance;
#pragma endregion


#pragma region Gravity
	UPROPERTY(EditAnywhere, Category = "Parameters|Gravity", meta = (EditCondition = "!useCharge", EditConditionHides))
	float initialGravityScale;

	UPROPERTY(EditAnywhere, Category = "Parameters|Gravity", meta = (EditCondition = "!useCharge", EditConditionHides))
	float finalGravityScale;
#pragma endregion

#pragma endregion

#pragma region Charge
	UPROPERTY(EditAnywhere, Category = "Parameters|Force", meta = (EditCondition = "useCharge", EditConditionHides))
	float minInitialZVelocityBoost;

	UPROPERTY(EditAnywhere, Category = "Parameters|Force", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxInitialZVelocityBoost;


	UPROPERTY(EditAnywhere, Category = "Parameters|Speed", meta = (EditCondition = "useCharge", EditConditionHides))
	float minInitialSpeed;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxInitialSpeed;


#pragma region Resistance
	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "useCharge", EditConditionHides))
	float minDistanceSwitch;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxDistanceSwitch;


	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "useCharge", EditConditionHides))
	float minInitialResistance;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxInitialResistance;


	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "useCharge", EditConditionHides))
	float minFinalResistance;

	UPROPERTY(EditAnywhere, Category = "Parameters|Speed|Resistance", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxFinalResistance;
#pragma endregion


#pragma region Gravity
	UPROPERTY(EditAnywhere, Category = "Parameters|Gravity", meta = (EditCondition = "useCharge", EditConditionHides))
	float minInitialGravityScale;

	UPROPERTY(EditAnywhere, Category = "Parameters|Gravity", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxInitialGravityScale;


	UPROPERTY(EditAnywhere, Category = "Parameters|Gravity", meta = (EditCondition = "useCharge", EditConditionHides))
	float minFinalGravityScale;

	UPROPERTY(EditAnywhere, Category = "Parameters|Gravity", meta = (EditCondition = "useCharge", EditConditionHides))
	float maxFinalGravityScale;
#pragma endregion

#pragma endregion


	UPROPERTY()
	TObjectPtr<AMainCharacter> mainCharacter;


#pragma region Member
	float chargeTime;
	float maxChargeTime;
#pragma endregion

	bool canMove;
	float currentDistanceSwitch;
	float currentResistance;
	float currentGravityScale;

	float bonusDamage;
	float bleedRate;
	FStatusParams bleedBonusParams;

	FVector currentVelocity;
	FVector startLocation;

	FHitResult moveResult;

public:
	FORCEINLINE void SetBonusDamage(const float _damage) { bonusDamage = _damage; }

	FORCEINLINE void SetBleedBonusParams(const float _value, const float _duration, const float _bleedRate)
	{
		bleedBonusParams.value = _value;
		bleedBonusParams.duration = _duration;
		bleedRate = _bleedRate;
	}

public:
	AProjectileActor();

private:
	void AddInitialZVelocityBoost(const float _chargeTime = 0.0f, const float _maxChargeTime = 0.0f);

	void UpdateParametersByDistance();
	void UpdateCurrentVelocity(const float _deltaTime);
	void Move(const float _deltaTime);

	float GetValueFromCharge(const float _chargeTime, const float _maxChargeTime, const float _minValue, const float _maxValue) const;

protected:
	void AddBleed(AActor* _target);

	virtual void Tick(float DeltaTime) override;
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void StopIfHit();

public:
	virtual void InitProjectile(AMainCharacter* _mainCharacter, const float _chargeTime = 0.0f, const float _maxChargeTime = 0.0f, const FVector _charaVelocity = FVector(0.0f));
};
