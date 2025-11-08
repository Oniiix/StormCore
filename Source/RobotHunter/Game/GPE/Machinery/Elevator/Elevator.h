#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "Components/SplineComponent.h"
#include "Elevator.generated.h"


UENUM()
enum ETypeEase
{
	EASEIN,
	EASEOUT,
	DEFAULT,
};

UCLASS()
class ROBOTHUNTER_API AElevator : public AReceiver
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnStopMove);

	FOnStopMove onStopMove;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> spline = nullptr;
	UPROPERTY(EditAnywhere)
	float speed = 50;
	UPROPERTY(EditAnywhere)
	float timeStop = 2;
	//UPROPERTY(EditAnywhere)
	//TEnumAsByte<ETypeEase> typeEase = DEFAULT;

	UPROPERTY(EditAnywhere);
	float initIndex = 0;

	float currentIndex = 0;
	float maxNbrPoint = 0;

	float timeTypeTimer = 0;

	bool returnMode = false;
	bool canStopElevator = false;

	UPROPERTY()
	TArray<FVector> positionAllPoint;

	UPROPERTY()
	FVector currentTargetPoint;
	UPROPERTY()
	FVector initPointLocation;

	FTimerHandle timerStop, timerTypeTimer;

public:
	AElevator();

#pragma region Getter
	FORCEINLINE FOnStopMove& OnStopMove() { return onStopMove; }

	FORCEINLINE float GetSpeed()const
	{
		return speed;
	}
	FORCEINLINE float GetTimeStop()const
	{
		return timeStop;
	}
	FORCEINLINE float GetCurrentIndex()const
	{
		return currentIndex;
	}
	FORCEINLINE float GetMaxNbrPoint()const
	{
		return maxNbrPoint;
	}
	FORCEINLINE bool GetReturnMode()const
	{
		return returnMode;
	}
	FORCEINLINE FVector GetCurrentTargetPoint()const
	{
		return currentTargetPoint;
	}
	FORCEINLINE FVector GetInitPoint()const
	{
		return initPointLocation;
	}
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetSpeed(const float _speed)
	{
		speed = _speed;
	}
	FORCEINLINE void SetTimeStop(const float _timeStop)
	{
		timeStop = _timeStop;
	}
	FORCEINLINE void SetCurrentTargetPoint(const FVector& _currentTargetPoint)
	{
		currentTargetPoint = _currentTargetPoint;
	}
	FORCEINLINE void SetInitPoint(const FVector& _initPointLocation)
	{
		initPointLocation = _initPointLocation;
	}
#pragma endregion

protected:
	virtual void InitReceiver() override;
	virtual void Tick(float _deltaTime)override;

	/// <summary>
	/// Activate elevator
	/// </summary>
	UFUNCTION() virtual void Action() override;

private:
#pragma region Move
	/// <summary>
	/// Move Evealtor
	/// </summary>
	void Move();
	/// <summary>
	/// Change target for Move
	/// </summary>
	void ChangePoint();
#pragma endregion

	void InitElevator();
	void BindEvent();

	UFUNCTION() void ActionSwitch();
	UFUNCTION() void ActionTimer();

	UFUNCTION() void StopMoveSwitch();
	UFUNCTION() void StopMoveTimer();
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
