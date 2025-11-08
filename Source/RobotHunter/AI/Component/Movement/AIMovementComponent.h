// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "RobotHunter/AI/Arena/Component/AIRangePosition/AIRangePositionComponent.h"
#include "AIMovementComponent.generated.h"

#define AI_MOVE_COMPONENT_USE_LOG false

class AAI_Base;
class UMovableComponent;
class UCharacterMovementComponent;
class UAIBaseAnimInstance;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UAIMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove, int, _axis);
	FOnMove onMove;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJump, bool, _isJump);
	FOnJump onJump;
	UPROPERTY(Replicated)
	FVector newLocation = FVector::ZeroVector;

	UPROPERTY(Replicated)
	TObjectPtr<AAIController> controller = nullptr;

	UPROPERTY(EditAnywhere, Category = "AIMovement_Property", meta = (UMin = 0, ClampMin = 0))
	int walkSpeed = 400;
	UPROPERTY(EditAnywhere, Category = "AIMovement_Property", meta = (UMin = 0, ClampMin = 0))
	int runSpeed = 800;
	UPROPERTY(EditAnywhere, Category = "AIMovement_Property", meta = (UMin = 0, ClampMin = 0))
	int acceleration = 100;
	UPROPERTY(EditAnywhere, Category = "AIMovement_Property", meta = (UMin = 0, ClampMin = 0))
	int rotationSpeed = 360;
	UPROPERTY(EditAnywhere, Category = "AIMovement_Property")
	bool drawDebug = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIMovement_Property", meta = (AllowPrivateAccess))
	bool canMoveInRange = false;
	UPROPERTY(EditAnywhere, Category = "AIMovement_Property")
	TArray<TEnumAsByte<EObjectTypeQuery>> layersGround;

	TObjectPtr<AAI_Base> owner = nullptr;
	TObjectPtr<UCharacterMovementComponent> characterMovementComponent = nullptr;
	TObjectPtr<UMovableComponent> movableComponent = nullptr;
	TObjectPtr<UAIRangePositionComponent> rangePositionArena = nullptr;
	TObjectPtr<UAIBaseAnimInstance> AIBaseAnimInstance = nullptr;
	FPointRangeArena targetPoint;

	/// To play animation in client
	UPROPERTY(ReplicatedUsing = OnRepMove)
	bool canMove = false;

	float currentSpeedWalk = 0, currentSpeedRun = 0;
	float speedMultiplier = 1;
	float halfheight = 0.f;

	bool isRun = false;
	bool lockingMovement = false;
	float countDeltaTime = 0;
	FAIRequestID requestId;

	bool isInitializationGood = false;

public:
	UAIMovementComponent();

#pragma region Getter 
	FORCEINLINE FOnMove& OnMove() { return onMove; }
	FORCEINLINE FOnJump& OnJump() { return onJump; }
	FORCEINLINE int GetWalkSpeed() const { return walkSpeed; }
	FORCEINLINE int GetRunSpeed() const { return runSpeed; }
	FORCEINLINE int GetAcceleration() const { return acceleration; }
	FORCEINLINE bool GetCanMoveInRange() const { return canMoveInRange; }
	FORCEINLINE FPointRangeArena GetRangePoint() const { return targetPoint; }
	FORCEINLINE TArray<TEnumAsByte<EObjectTypeQuery>> GetLayersGround() const { return layersGround; }
	FORCEINLINE float GetCurrentSpeedRun() const { return currentSpeedRun; }
	FORCEINLINE float GetSpeedCurrentSpeedWalk() const { return currentSpeedWalk; }
	FORCEINLINE bool GetIsRun() const { return isRun; }
	/// <summary>
	/// Get a point and available the last point
	/// </summary>
	/// <param name="_typeRange"> type target range </param>
	/// <param name="_mainCharacter"> player target </param>
	/// <param name="_canCheckLine"> if is true check if the line </param>
	/// <param name="_offSetShoot"></param>
	/// <returns> point position </returns>
	FVector GetNewTargetPointRange(const ERangeAttack& _typeRange, const AMainCharacter* _mainCharacter, const bool _canCheckLine = false, const float _offSetShoot = 200);
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetRunSpeed(const int _runSpeed) { runSpeed = _runSpeed; }
	FORCEINLINE void SetWalkSpeed(const int _walkSpeed) { walkSpeed = _walkSpeed; }
	FORCEINLINE void SetAcceleration(const int _acceleration) { acceleration = _acceleration; }
	FORCEINLINE void SetRotationSpeed(const int _rotationSpeed) { rotationSpeed = _rotationSpeed; }
	FORCEINLINE void SetDrawDebug(const bool _drawDebug) { drawDebug = _drawDebug; }
	FORCEINLINE void SetCanMoveInRange(const bool _canMoveInRange) { canMoveInRange = _canMoveInRange; }
	FORCEINLINE void SetCurrentSpeedRun(const float _speed) { currentSpeedRun = _speed; }
	FORCEINLINE void SetCurrentSpeedWalk(const float _walk) { currentSpeedWalk = _walk; }
	FORCEINLINE void SetLayersGround(TArray<TEnumAsByte<EObjectTypeQuery>> _layersGround) { layersGround = _layersGround; }
	/// <summary>
	/// Set available last targetPoint and set targetPoint by targetpoint
	/// </summary>
	/// <param name="_newPoint"> target point </param>
	FORCEINLINE void SetTargetPoint(const FPointRangeArena& _newPoint) { rangePositionArena->SetAvailablePoint(targetPoint, true);		targetPoint = _newPoint; }
#pragma endregion

	/// <summary>
	/// Init component
	/// </summary>
	void InitAIMovement();
	/// <summary>
	/// Update speed of characterMovement
	/// </summary>
	/// <param name="_value"> if is true is speedRun else walkSpeed </param>
	void UpdateSpeed(const bool _value);
	/// <summary>
	///  Update acceleration of characterMovement
	/// </summary>
	void UpdateAcceleration();
	/// <summary>
	/// Reset speed walk and run
	/// </summary>
	void ResetAllSpeed();
	/// <summary>
	/// Reset run speed 
	/// </summary>
	void ResetRunSpeed();
	/// <summary>
	/// Reset walk speed
	/// </summary>
	void ResetWalkSpeed();

	/// <summary>
	/// Move to location
	/// </summary>
	/// <param name="_newLocation"> position to move </param>
	/// <param name="_acceptanceRadius"> radius for stop ai </param>
	void Move(const FVector& _newLocation, const float _acceptanceRadius = -1);
	/// <summary>
	/// Move to location
	/// </summary>
	/// <param name="_newLocation"> position to actor </param>
	/// <param name="_acceptanceRadius"> radius for stop ai </param>
	void Move(AActor* _target, const float _acceptanceRadius = -1);
	/// <summary>
	/// Stop all Move AI
	/// </summary>
	void StopMove();
	/// <summary>
	/// Jump with NavMesh
	/// </summary>
	/// <param name="_outVelocity"> Velocity for jump </param>
	/// <param name="_boost"> ration distance between jump </param>
	UFUNCTION(BlueprintCallable) 
	void EndJump();

	UFUNCTION()
	void LockMovement();
	UFUNCTION()
	void UnlockMovement();

	void LookAtTarget(const FVector& _target);

	UFUNCTION() 
	void RefreshPoint();

	void SetSpeedMultiplier(const float _modifier, const float _percent);
	void ResetSpeedMultiplier();

private:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FRotator UpdateRotationToGround(float DeltaTime);
	FRotator UpdateRotationToMovement(float DeltaTime);

	/// <summary>
	/// Debug
	/// </summary>
	void DrawDebug();

	void ModifierSpeed(const float _multiplier);
	UFUNCTION() 
	void ResetPointNav();
	UFUNCTION() 
	void OnRepMove();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
