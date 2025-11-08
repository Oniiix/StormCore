// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "Components/SphereComponent.h"
#include "FireFliesProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AFireFliesProjectile : public AActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, const FHitResult&, _result);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowprivateAccess))
	FOnHit onHit;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float lifeSpan = 3;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float lifeSpanFX = 1;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float speed = 700;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Curve|Range")
	float maxRangeCurve = 500;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Curve|Range")
	float minRangeCurve = -500;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Curve")
	float distanceOfTarget = 100;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Curve|Angle", meta = (Min = 0, Max = 90, ClampMin = 0, ClampMax = 90))
	float minAngleCurve = 0;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Curve|Angle", meta = (Min = 0, Max = 90, ClampMin = 0, ClampMax = 90))
	float maxAngleCurve = 50;
	float topCurve = 0;
	float angleCurve = 0;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	float maxRangeMesh = 100;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	float speedMesh = 500;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Mesh")
	TObjectPtr<USphereComponent> fireCollision = nullptr;

	UPROPERTY()
	TArray<FVector> allPoints;
	
	UPROPERTY()
	TObjectPtr<AActor> target = nullptr;

	UPROPERTY()
	TObjectPtr<AMainCharacter> player = nullptr;


	float damage = 10;

	float bonusDamage;
	float bleedRate;
	FStatusParams bleedBonusParams;

	float index = 0;

	bool isInit = false;

	FVector direction, middleCurve, meshDestination;

	bool canHit = false;

	FHitResult moveResult;

public:
	AFireFliesProjectile();

#pragma region Getter
	FORCEINLINE FOnHit& OnHit() { return onHit; }
	FORCEINLINE USphereComponent* GetFireCollision()const
	{
		return fireCollision;
	}
	FORCEINLINE float GetSpeed()const
	{
		return speed;
	}
	FORCEINLINE float GetDamage()const
	{
		return damage;
	}
	FORCEINLINE AActor* GetTarget()const
	{
		return target;
	}
	FORCEINLINE FVector GetDirection()const
	{
		return direction;
	}
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetSpeed(const float& _speed)
	{
		speed = _speed;
	}
	FORCEINLINE void SetDamage(const float& _damage)
	{
		damage = _damage;
	}
	FORCEINLINE void SetTarget(AActor* _target)
	{
		target = _target;
	}
	FORCEINLINE void SetCurveValue(const float _topCurve, const float _angleCurve)
	{
		topCurve = _topCurve;
		angleCurve = _angleCurve;
	}
	FORCEINLINE void SetDirection(const FVector& _direction)
	{
		direction = _direction;
	}
	FORCEINLINE void SetPlayer(AMainCharacter* _player)
	{
		player = _player;
	}
	FORCEINLINE void SetAllPoint(const TArray<FVector>& _allPoints)
	{
		allPoints = _allPoints;
	}

	FORCEINLINE void SetBonusDamage(const float _bonus) { bonusDamage = _bonus; }

	FORCEINLINE void SetBleedBonusParams(const float _value, const float _duration, const float _bleedRate)
	{
		bleedBonusParams.value = _value;
		bleedBonusParams.duration = _duration;
		bleedRate = _bleedRate;
	}
#pragma endregion

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& _propertyChangedEvent) override;
#endif 
	virtual void BeginPlay()override;
	virtual void Tick(float _deltaTime)override;

private:
	void MoveActor(const AActor* _target, const FVector& _actorLocation);
	void MoveToTarget(const AActor* _target, const FVector& _actorLocation);
	void MoveCurve();
	void MoveDirection(const FVector& _direction, const FVector& _actorLocation);

	void AddBleed(AActor* _target);

public:
	void InitActor(const bool _canHit);

	TPair<float, float> RandomValue();
private:
	void SetMiddleCurve(const FVector& _goalLocation, const FVector& _baseLocation);
	TArray<FVector> CreateCurve(const FVector& _goalPosition)const;
	FVector GetMovementCurve(const FVector& _pos1, const FVector& _pos2, const FVector& _pos3, const float _time)const;

	UFUNCTION() void TargetDie();

	//UFUNCTION() void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void MakeDamage(const FHitResult& _result);
};
