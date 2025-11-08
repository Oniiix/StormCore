#pragma once

#include "CoreMinimal.h"
#include "../CustomSplineActor.h"	
#include "Components/ArrowComponent.h"
#include "RailActor.generated.h"

class ALeverActor;
class ARailsTargetActor;

/*The rail has two directions : the departure direction and the return direction. The departure direction starts from the starting point of the level
and goes toward the end of the level. The return direction starts from the end of the level and goes toward the starting point of the level.*/

UCLASS()
class ROBOTHUNTER_API ARailActor : public ACustomSplineActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlockedRailDetected);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPathRailDetected);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRailNotDetectedAnymore);


	UPROPERTY(EditAnywhere, Category = "Debug")
	float debugSphereRadius;


	UPROPERTY(EditAnywhere, Category = "Arrow")
	TObjectPtr<UArrowComponent> arrowComponent;


	UPROPERTY(EditAnywhere, Category = "Lock")
	bool isLocked;

	
	/// <summary>
	/// On which point the wagon should stops when it's blocked (departure direction).
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Stop", meta = (UIMin = 0, ClampMin = 0))
	int departureStopIndex;

	/// <summary>
	/// On which point the wagon should stops when it's blocked (return direction).
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Stop", meta = (UIMin = 0, ClampMin = 0))
	int returnStopIndex;


	/*UPROPERTY(EditAnywhere, Category = "Levers")
	TSubclassOf<ALeverActor> leverType;

	UPROPERTY(EditAnywhere, Category = "Levers")
	TObjectPtr<ALeverActor> departureLever;

	UPROPERTY(EditAnywhere, Category = "Levers")
	TObjectPtr<ALeverActor> returnLever;*/

	UPROPERTY(EditAnywhere, Category = "Target")
	TSubclassOf<ARailsTargetActor> targetType;

	UPROPERTY(EditAnywhere, Category = "Target")
	TObjectPtr<ARailsTargetActor> departureTarget;

	UPROPERTY(EditAnywhere, Category = "Target")
	TObjectPtr<ARailsTargetActor> returnTarget;


	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FBlockedRailDetected blockedRailDetected;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnPathRailDetected onPathRailDetected;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FRailNotDetectedAnymore railNotDetectedAnymore;


	/// <summary>
	/// If this rail can be left when the wagon is on the departure direction.
	/// </summary>
	bool isBlockedOnDepartureExit;
	/// <summary>
	/// If this rail can be left when the wagon is on the return direction.
	/// </summary>
	bool isBlockedOnReturnExit;

public:
	/*FORCEINLINE void SetReturnLever(ALeverActor* _lever) { returnLever = _lever; }
	FORCEINLINE void SetDepartureLever(ALeverActor* _lever) { departureLever = _lever; }
	FORCEINLINE void SetIsBlocked(ALeverActor* _lever, const bool _isBlocked)
	{
		if (_lever == departureLever)
			isBlockedOnDepartureExit = _isBlocked;
		else if (_lever == returnLever)
			isBlockedOnReturnExit = _isBlocked;
	}*/

	FORCEINLINE void SetReturnTarget(ARailsTargetActor* _target) { returnTarget = _target; }
	FORCEINLINE void SetDepartureTarget(ARailsTargetActor* _target) { departureTarget = _target; }
	FORCEINLINE void SetIsBlocked(ARailsTargetActor* _target, const bool _isBlocked)
	{
		if (_target == departureTarget)
			isBlockedOnDepartureExit = _isBlocked;
		else if (_target == returnTarget)
			isBlockedOnReturnExit = _isBlocked;
	} 

	FORCEINLINE bool GetIsLocked() const { return isLocked; }
	/*FORCEINLINE bool GetIsBlocked(ALeverActor* _lever) const
	{
		if (_lever == departureLever)
			return isBlockedOnDepartureExit;
		else if (_lever == returnLever)
			return isBlockedOnReturnExit;

		return true;
	}*/
	FORCEINLINE bool GetIsBlocked(const bool _isOnReturn, const bool _isNextRail) const
	{
		if (!_isNextRail)
			return _isOnReturn ? isBlockedOnReturnExit : isBlockedOnDepartureExit;
		else
			return _isOnReturn ? isBlockedOnDepartureExit : isBlockedOnReturnExit;
	}

	FORCEINLINE bool GetIsBlocked(ARailsTargetActor* _target) const
	{
		if (_target == departureTarget)
			return isBlockedOnDepartureExit;
		else if (_target == returnTarget)
			return isBlockedOnReturnExit;

		return true;
	}

	FORCEINLINE int GetStopIndex(const bool _isDepartureLever) const
	{
		const int _index = _isDepartureLever ? (departureStopIndex - 1) : (returnStopIndex - 1);
		return (_index < 0) ? 0 : (_index > GetNumberOfPoints()) ? GetNumberOfPoints() : _index;
	}

	FORCEINLINE FVector GetStopLocationOnDepartureExit() const
	{
		return GetLocationAtPointIndex(GetStopIndex(true));
	}
	FORCEINLINE FVector GetStopLocationOnReturnExit() const
	{
		return GetLocationAtPointIndex(GetStopIndex(false));
	}
	FORCEINLINE FVector GetStopLocationOnExit(const bool _isOnReturn) const
	{
		return _isOnReturn ? GetStopLocationOnReturnExit() : GetStopLocationOnDepartureExit();
	}

	/*FORCEINLINE ALeverActor* GetDepartureLever() const { return departureLever; }
	FORCEINLINE ALeverActor* GetReturnLever() const { return returnLever; }
	FORCEINLINE ALeverActor* GetNextLever(const float _isOnReturn) const
	{
		return _isOnReturn ? returnLever : departureLever;
	}*/

	FORCEINLINE ARailsTargetActor* GetDepartureTarget() const { return departureTarget; }
	FORCEINLINE ARailsTargetActor* GetReturnTarget() const { return returnTarget; }
	FORCEINLINE ARailsTargetActor* GetNextTarget(const float _isOnReturn) const
	{
		return _isOnReturn ? returnTarget : departureTarget;
	}

	FORCEINLINE FBlockedRailDetected& GetBlockedRailDetected() { return blockedRailDetected; }
	FORCEINLINE FOnPathRailDetected& GetOnPathRailDetected() { return onPathRailDetected; }
	FORCEINLINE FRailNotDetectedAnymore& GetRailNotDetectedAnymore() { return railNotDetectedAnymore; }

public:
	ARailActor();

private:
	//ALeverActor* SpawnLever(const bool _isDepartureLever);
	//ALeverActor* SpawnReverseLever(const bool _isDepartureLever);

	ARailsTargetActor* SpawnTarget(const bool _isDepartureTarget);

	void UpdateArrowLocRot();

protected:
	virtual void DrawDebug() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SnapSplinePoint() override;

public:
	//UFUNCTION(CallInEditor, Category = "Levers") void SpawnDepartureLever();
	//UFUNCTION(CallInEditor, Category = "Levers") void SpawnReverseDepartureLever();
	//UFUNCTION(CallInEditor, Category = "Levers") void SpawnReturnLever();
	//UFUNCTION(CallInEditor, Category = "Levers") void SpawnReverseReturnLever();

	UFUNCTION(CallInEditor, Category = "Targets") void SpawnDepartureTarget();
	UFUNCTION(CallInEditor, Category = "Targets") void SpawnReturnTarget();
};
