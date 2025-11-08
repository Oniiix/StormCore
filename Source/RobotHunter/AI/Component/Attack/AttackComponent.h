// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include <Components/BoxComponent.h>
#include "RobotHunter/Utils/ENUM_Projectile.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"
#include "AttackComponent.generated.h"

#define ATTACK_COMPONENT_USE_LOG false
#define CUSTOM_TICK 0.5f
#define SHOOT_SOCKECT "Shoot_Socket"

class AAI_Base;
class AArena;
class AAIController;
class UAIBaseAnimInstance;
class UAIMovementComponent;
class UMovableComponent;
class UTrackingSystemComponent;
class AMainCharacter;
class IMainCharacterInterface;
class AttackCQCDataAsset;
class UThreatReasonDataAsset;
class UAttackCQCDataAsset;
struct FRangeAttack;

USTRUCT()
struct FAIRangeAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0))
	float cooldown = 1.f;
	UPROPERTY(VisibleAnywhere)
	bool isInCooldown = false;

	FTimerHandle timerCooldown;

	FAIRangeAttack() = default;
	FAIRangeAttack(const float _cooldown) : cooldown(_cooldown), isInCooldown(false)
	{}
};

USTRUCT()
struct FProjectilePrecision
{
	GENERATED_BODY()
	
public:
	int currentPrecision;
	int maxPrecision;
	int addPrecision ;

	FProjectilePrecision()
	{
		currentPrecision = 0;
		maxPrecision = 100;
		addPrecision = 10;
	}
	FProjectilePrecision(const int _initPrecision, const int _maxPrecision, const int _addPrecision) :
		currentPrecision(_initPrecision), maxPrecision(_maxPrecision), addPrecision(_addPrecision) 
	{}

	void AddPrecision()
	{
		if (currentPrecision >= maxPrecision)
			return;
		currentPrecision = FMath::Clamp(currentPrecision + addPrecision, 0, maxPrecision);
	}
};

/*
* The component that handle AI's attack (CQC and distance)
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
	FOnAttack onAttack;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAttack);
	FOnEndAttack onEndAttack;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChangeTarget, FUniqueNetIdRepl, _oldTarget, FUniqueNetIdRepl, _newTarget, AAI_Base*, _AI);
	FOnChangeTarget onChangeTarget;

#pragma region Properties
	UPROPERTY(VisibleAnywhere, Category = "AttackComponent_Property")
	TObjectPtr<UShapeComponent> boxCQC = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "AttackComponent_Property")
	TObjectPtr<USceneComponent> offsetShoot = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "AttackComponent_Property")
	bool disableAttack = false;

	UPROPERTY(EditAnywhere, Category = "AttackComponent_Property|CQC", meta = (UMin = 0, ClampMin = 0, UMax = 1, ClampMax = 1))
	float pushSnapPower = 0.5;
	UPROPERTY(EditAnywhere, Category = "AttackComponent_Property|CQC", meta = (UMin = 0, ClampMin = 0))
	int moveDistance = 200;

	TObjectPtr<AAI_Base> owner = nullptr;
	TObjectPtr<AArena> arena = nullptr;
	TObjectPtr<UAIBaseAnimInstance> animInstance = nullptr;
	TObjectPtr<UAIMovementComponent> AIMovementComponent = nullptr;
	TObjectPtr<UMovableComponent> movableComponent = nullptr;
	TObjectPtr<UTrackingSystemComponent> trackingSystem = nullptr;
	TSortedMap<TEnumAsByte<ERangeAttack>, FAIRangeAttack> mapRange;

	TSubclassOf<AProjectile> projectileRef;
	TMap<TSubclassOf<AProjectile>, FProjectilePrecision> precisionProjectile;

	TObjectPtr<AAIController> AIController = nullptr;
	TObjectPtr<UAttackCQCDataAsset> currentCQCData = nullptr;
	FName defaultCollisionProfileCQC;
	TArray<TObjectPtr<AActor>> listActorsHitByCQC;

	TObjectPtr<UProjectileStats> currentProjectileStat = nullptr;

	FTimerHandle timerShoot;
	int currentNumberOfShoot = 0;
	int numberOfProjectile = 0;

	FVector2D imprecisionFactor;

	bool isInitializationGood = false;
#pragma endregion

#pragma region Methods
public:
	UAttackComponent();

	FORCEINLINE FOnAttack& OnAttack() { return onAttack; }
	FORCEINLINE FOnEndAttack& OnEndAttack() { return onEndAttack; }
	FORCEINLINE FOnChangeTarget& OnChangeTarget() { return onChangeTarget; }
	FORCEINLINE TSortedMap<TEnumAsByte<ERangeAttack>, FAIRangeAttack> GetMapRange() { return mapRange; }
	FORCEINLINE bool IsInCooldown(const ERangeAttack& _rangeAttack) { return mapRange[_rangeAttack].isInCooldown; }

	FORCEINLINE void SetDisableAttack(const bool _value) { disableAttack = _value; }
	FORCEINLINE void SetPushSnapPower(const float _value) { pushSnapPower = _value; }
	FORCEINLINE void SetMoveDistance(const int _value) { moveDistance = _value; }
	FORCEINLINE void SetImprecisionFactor(const FVector2D& _value) { imprecisionFactor = _value; }
	void SetListRange(const TArray<FRangeAttack>& _listRange);

	/// <summary>
	/// Initialize the component
	/// </summary>
	void InitAttack();
	UFUNCTION()
	/// <summary>
	/// Reset attack component
	/// </summary>
	void ResetAttack();

	void InitCQCAttack(UAttackCQCDataAsset* _CQCData);
	UFUNCTION()
	void EnableHitBox();
	UFUNCTION()
	void DisableHitBox();
	UFUNCTION()
	void JumpAttack();
	void EndAttackCQC();

	/// <summary>
	/// Spawn projectile
	/// </summary>
	/// <param name="_target"></param>
	void AttackDistance(TSubclassOf<AProjectile> _projectileRef);

	void StartRangeTimer(const ERangeAttack& _range);
	UFUNCTION()
	void EndRangeTimer(const ERangeAttack& _range);

	/// <summary>
	/// Get the player (in mapPlayersThreat) with the highest threat for AI owner
	/// </summary>
	/// <returns> the player ref </returns>
	FUniqueNetIdRepl GetPlayerIdWithHighestThreat();

	AMainCharacter* GetCharacterByNetId(FUniqueNetIdRepl _id);

private:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// InitAnimInstance BeginOverlap of BoxCQC
	/// </summary>
	UFUNCTION()
	void BoxCQCBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// Predict the position of a target thanks to this velocity
	/// </summary>
	/// <param name="_positionTarget"></param>
	/// <param name="_velocityTarget"></param>
	/// <param name="_speedProjectile"></param>
	/// <returns></returns>
	FVector PredictedLocation(const FVector& _targetLocation, const FVector& _targetVelocity, const float _actorSpeed = 1.f);
	/// <summary>
	/// return a point with a precision rate
	/// </summary>
	/// <param name="_position">exact position</param>
	/// <returns></returns>
	FVector PrecisionLocation(const FVector& _position, const int _currentPrecision) const;

	UFUNCTION()
	void StartAttack();
	UFUNCTION()
	/// <summary>
	/// Spawn additional Projectile
	/// </summary>
	void SpawnProjectile();

public:
	void ClearTimerProjectile();
#pragma endregion
};
