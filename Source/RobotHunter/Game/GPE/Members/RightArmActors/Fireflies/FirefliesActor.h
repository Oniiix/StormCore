// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/RightArmActor.h"
#include "DroneFireflies.h"
#include "RobotHunter/Game/GPE/Projectile/FireFliesProjectile/FireFliesProjectile.h"
#include "RobotHunter/UI/ATH/Reticle/RightArm/Fireflies/FireFliesReticle.h"
#include <RobotHunter/UI/HUD/GameHUD.h>
#include "FirefliesActor.generated.h"

/**
 * 
 */
UCLASS(HideDropdown, HideCategories = ("Charge"))
class ROBOTHUNTER_API AFirefliesActor : public ARightArmActor
{
	GENERATED_BODY()

#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirefliesShoot, const float, _percentage);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnFirefliesShoot onFirefliesShoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisableEnemy, AActor*, _disableEnemy);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnDisableEnemy onDisableEnemy;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams (FOnStartMark, AActor*, _target, const float, _time);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStartMark onStartMark;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnCurrentMark, float, _coeff);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnCurrentMark onCurrentMark;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMark, const int, _currentMark, AActor*, _actorMark);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnMark onMark;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopMark, AActor*, _target);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStopMark onStopMark;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverheat, const bool, _isOverheat);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnOverheat onOverheat;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerfectCool, const bool, _isCool);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnPerfectCool onPerfectCool;

#pragma region Song
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSingleShoot);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnSingleShoot onSingleShoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAutoShoot);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnAutoShoot onAutoShoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMakConfirmed);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnMakConfirmed onMakConfirmed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAutoRelease);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnAutoRelease onAutoRelease;
#pragma endregion

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies")
	TSubclassOf<ADroneFireflies> drone = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies")
	TSubclassOf<AFireFliesProjectile> projectileRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies")
	float range = 1000;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies")
	TArray<TEnumAsByte< EObjectTypeQuery>> layerShoot;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies")
	TArray<TEnumAsByte< EObjectTypeQuery>> layerObstacle;

	UPROPERTY(EditAnywhere,Category = "Custom|Property|FireFlies|Angles")
	float percOverheatMinReticle = 80;

	float overheatMaxReticle = 0;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Cooling")
	float timeCooling = 3.f;

#pragma region Overheat
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Overheat")
	float maxOverheated = 10.f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Overheat")
	float overheatMaxToShoot = 1.f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Overheat")
	float overheatMinToShoot = 1.f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Overheat")
	float speedDownOverheat = 10;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Overheat")
	float timeCanOverheat = 0.5;

	float currentOverheat = 0.f;
	bool isOverheat = false;
	bool canDownOverheat = false;
#pragma endregion

#pragma region Mark
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float durationMark = 5.f;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark")
	float maxMark = 2;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark", BlueprintReadWrite, meta = (AllowPrivateAccess))
	float timeEnemyMark = 1;
	float currentMark;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark")
	float distanceMin = 200;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark")
	float distanceMax = 2000;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark")
	float timeMinDistance = 0.1;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies|Mark")
	float timeMaxDistance = 0.5;
#pragma endregion


	UPROPERTY(EditAnywhere, Category = "Custom|Property|FireFlies")
	float timeSingleShoot = 0.0001;

	UPROPERTY()
	TArray<AActor*> allEnemyMark;

	UPROPERTY()
	TArray<AActor*> allEnemyInView;
	int currentEnemyView = 0;

	TArray<AActor*> listTargetAim;

	UPROPERTY()
	TArray<ADroneFireflies*> allDrone;

	float timeInTimerCooling = 0;

	FTimerHandle timerCooling, timerMark, timerOverheat;

	int targetIndex = -1;

	TObjectPtr<UCameraComponent> cam = nullptr;
	TObjectPtr<UFirefliesCrosshair> reticle = nullptr;

	bool firstAim = false;



	UPROPERTY()
	TObjectPtr<AGameHUD> gameHUD = nullptr;

	bool hasAuthority = false;

	int currentShoot = 0;

	FTimerHandle timerSingleShoot;
	bool canTimerSingleShoot = true;

public:
#pragma region EventMethode
	FORCEINLINE FOnFirefliesShoot& OnFirefliesShoot()
	{
		return onFirefliesShoot;
	}
	FORCEINLINE FOnMark& OnMark()
	{
		return onMark;
	}
	FORCEINLINE FOnOverheat& OnOverheat()
	{
		return onOverheat;
	}
	FORCEINLINE FOnStartMark& OnStartMark()
	{
		return onStartMark;
	}
	FORCEINLINE FOnStopMark& OnStopMark()
	{
		return onStopMark;
	}
	FORCEINLINE FOnPerfectCool& OnPerfectCool()
	{
		return onPerfectCool;
	}
	FORCEINLINE FOnDisableEnemy& OnDisableMark()
	{
		return onDisableEnemy;
	}
#pragma endregion

#pragma region Getter
	FORCEINLINE float GetCurrentOverheat()const
	{
		return currentOverheat;
	}
	FORCEINLINE float GetMaxOverheat()const
	{
		return maxOverheated;
	}
	FORCEINLINE float GetMaxMark()const
	{
		return maxMark;
	}
	FORCEINLINE float GetCurrentMark()const
	{
		return allEnemyMark.Num();
	}
	FORCEINLINE float GetDurationMark()const
	{
		return durationMark;
	}
	FORCEINLINE float GetTimeEnemyMark()const
	{
		return timeEnemyMark;
	}
	FORCEINLINE float GetTimeInTimerCooling()const
	{
		return timeInTimerCooling;
	}
	FORCEINLINE TObjectPtr<UFirefliesCrosshair> GetReticle()const 
	{
		return reticle;
	}
	
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetReticle(const TObjectPtr<UFirefliesCrosshair>& _reticle)
	{
		reticle = _reticle;
	}
#pragma endregion

protected:
	virtual void BeginPlay() override;
	/// <summary>
	/// Init member
	/// </summary>
	virtual void InitMember()override;
	virtual void Tick(float _deltaTime)override;

	/// <summary>
	/// active the system aim
	/// </summary>
	void AimMode();

	/// <summary>
	/// call when chara press shoot input
	/// </summary>
	virtual void PressBehavior() override;
	void Press_Internal();
	/// <summary>
	/// call when chara release shoot input 
	/// </summary>
	virtual void ReleaseBehavior() override;

	virtual bool CanUse() override;
	virtual void ResetMember() override;

public:
	/// <summary>
	/// Shoot to all target
	/// </summary>
	void ShootWithTarget();
	/// <summary>
	/// Shoot without target
	/// </summary>
	void ShootFirefliesWithoutTarget();

private:
	/// <summary>
	/// spawn fireflies
	/// </summary>
	/// <param name="_direction"> shoot direction </param>
	void Shoot(const FVector& _direction);
	/// <summary>
	/// spawn fireflies with target
	/// </summary>
	/// <param name="_direction"> direction target </param>
	/// <param name="_targetIndex"> index target </param>
	void Shoot(const FVector& _direction, const int _targetIndex);
	/// <summary>
	/// overheat weapon by shoot
	/// </summary>
	void Overheat();
	/// <summary>
	/// start system Mark
	/// </summary>
	/// <param name="_allResult"> all target </param>
	/// <param name="_direction">  </param>
	void MarkMode(const TArray<FHitResult>& _allResult);
	/// <summary>
	/// Mark confirmed
	/// </summary>
	/// <param name="_result"> result of LineTrace</param>
	UFUNCTION() void MarkEnemy(AActor* _actor, ADroneFireflies* _drone);
	/// <summary>
	/// Mark Enemy when arm shoot
	/// </summary>
	/// <param name="_enemy"> enemy marked</param>
	void SpawnDrone(AActor* _target);
	/// <summary>
	/// control the cooling
	/// </summary>
	void Cooling();
	/// <summary>
	/// Disable mark after enemy is dead or finish timeMark
	/// </summary>
	/// <param name="_enemy">enemy mark Disable</param>
	UFUNCTION() void DisableMark(AActor* _target, ADroneFireflies* _drone);
	/// <summary>
	/// Enemy out reticle
	/// </summary>
	/// <param name="_target"> enemy out view </param>
	UFUNCTION() void EnemyOutView(AActor* _target);
	/// <summary>
	/// Enemy in reticle
	/// </summary>
	/// <param name="_target"> enemy in view </param>
	UFUNCTION() void EnemyInView(AActor* _target);

	/// <summary>
	/// Clear all mark
	/// </summary>
	void ClearAllDrone();

	/// <summary>
	/// Spawn projectile
	/// </summary>
	/// <param name="_direction"> direction aim </param>
	/// <returns> projectile </returns>
	AFireFliesProjectile* InitProjectile(const FVector& _direction);

	void SongShootFireFlies();

	/// <summary>
	/// Show single shoot song
	/// </summary>
	void IsSingleShoot();

private:
#pragma region Reseau
	UFUNCTION(NetMulticast, Withvalidation,  Unreliable) void ClientRpc_SpawnFirefliesWithTarget(const FVector& _direction, const int _index, const float _topCurve, const float _angleCurve);
	UFUNCTION(Server, Unreliable) void ServerRpc_SpawnFirefliesWithTarget(const FVector& _direction, const int _index, const float _topCurve, const float _angleCurve);

	UFUNCTION(NetMulticast, Withvalidation,  Unreliable) void ClientRpc_SpawnFirefliesWithoutTarget(const FVector& _direction);
	UFUNCTION(Server, Unreliable) void ServerRpc_SpawnFirefliesWithoutTarget(const FVector& _direction);



	UFUNCTION(Server, Unreliable) void ServerRpc_AddEnemyInView(AActor* _actor);
	UFUNCTION(Server, Unreliable) void ServerRpc_RemoveEnemyInView(AActor* _actor);
	UFUNCTION(Server, Unreliable) void ServerRpc_ClearEnemyInView();

	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_AddEnemyInView(AActor* _actor);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_RemoveEnemyInView(AActor* _actor);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_ClearEnemyInView();

#pragma endregion

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
