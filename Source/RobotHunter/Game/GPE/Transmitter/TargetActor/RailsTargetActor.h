#pragma once

#include "CoreMinimal.h"
#include "TargetActor.h"
#include "RobotHunter/Splines/Rail/RailActor.h"
#include "RobotHunter/Splines/Rail/ChangeWayRailActor.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "Components/BoxComponent.h"
#include "RailsTargetActor.generated.h"

class ACustomHandcarActor;
class AStopHandcarActor;
class UNetworkGPEComponent;

UENUM()
enum ERailWay
{
	PositiveRailWay,
	NegativeRailWay,
	DefaultRailWay
}; 

UCLASS()
class ROBOTHUNTER_API ARailsTargetActor : public ACustomActor
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateRailIsBlocked);

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULifeComponent> lifeComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TSubclassOf<ARailActor> railType;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TSubclassOf<AChangeWayRailActor> changeWayRailType;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TArray<ARailActor*> returnRails;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TArray<ARailActor*> departureRails;

	UPROPERTY(EditAnywhere, Category = "Rails", meta = (UIMin = 1, ClampMin = 1))
	int startingIndex;


	UPROPERTY(EditAnywhere, Category = "AutoMode")
	TEnumAsByte<ERailWay> intersectionWay;

	UPROPERTY(EditAnywhere, Category = "AutoMode")
	TMap<AStopHandcarActor*, ARailActor*> autoModeNextRails;

	UPROPERTY()
	TObjectPtr<UNetworkGPEComponent> networkComp;


#pragma region Detection
	UPROPERTY(EditAnywhere, Category = "Detection")
	TObjectPtr<UBoxComponent> detectionSphere;

	UPROPERTY()
	TArray<ARailActor*> detectedRails;
#pragma endregion


	/*UPROPERTY(VisibleAnywhere)
	bool isReverse;*/

	UPROPERTY(ReplicatedUsing = OnRep_UpdateIndexReturn)
	int currentIndexReturn;

	UPROPERTY(Replicated = OnRep_UpdateIndexDeparture)
	int currentIndexDeparture;

	//bool isLinkedToHandcar;

	FOnUpdateRailIsBlocked onUpdateRailIsBlocked;


public:
	/// <summary>
	/// Set to true if the lever's bifurcations are towards the ending point (return direction).
	/// </summary>
	/// <param name="_value"></param>
	//FORCEINLINE void SetIsReverse(const bool _value) { isReverse = _value; }
	//FORCEINLINE void SetIsLinkedToHandcar(const bool _isLinkedToHandcar) { isLinkedToHandcar = _isLinkedToHandcar; }

	FORCEINLINE TEnumAsByte<ERailWay> GetIntersectionWay() const { return intersectionWay; }

	//FORCEINLINE bool GetIsLinkedToHandcar() const { return isLinkedToHandcar; }
	FORCEINLINE ARailActor* GetRailFromIndex(const int _index, const bool _isDepartureRail) const
	{
		if (_index < 0)
			return nullptr;

		if (_isDepartureRail)
		{
			if (departureRails.IsEmpty() || _index >= departureRails.Num())
				return nullptr;

			return departureRails[_index];
		}
		else
		{
			if (returnRails.IsEmpty() || _index >= returnRails.Num())
				return nullptr;

			return returnRails[_index];
		}
	}
	FORCEINLINE TArray<ARailActor*> GetDepartureRails() const { return departureRails; }

	FORCEINLINE FOnUpdateRailIsBlocked& GetOnUpdateRailIsBlocked() { return onUpdateRailIsBlocked; }

public:
	ARailsTargetActor();


private:
	void DrawDebugRails(const TArray<ARailActor*>& _rails, const int _currentIndex, const bool _useDepartureStop);

	void UpdateCurrentIndex(TArray<ARailActor*> _rails, int& _currentIndex, const bool _increment, const bool _clamp0 = true);
	void UpdateIndexes();
	void UpdateRailIsBlocked_Internal(TArray<ARailActor*>& _rails, int& _currentIndex, const bool _incrementIndex, const bool _updateIndex = true, const bool _clamp0 = true);

	void RefreshRails(TArray<ARailActor*>& _toRefreshRails);

	void SpawnRail_Internal(const bool _isDepartureRail);
	void SpawnChangeWayRail_Internal(const bool _isDepartureRail);


#pragma region Detection
	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RailIsOnHandcarPath(ARailActor* _rail);
#pragma endregion


protected:
	virtual void BeginPlay() override;
	virtual void DrawDebug() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION() virtual void ActivateTarget(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);

public:

	UFUNCTION(CallInEditor, Category = "Rails") void SpawnDepartureRail();
	//UFUNCTION(CallInEditor, Category = "Rails") void SpawnChangeWayDepartureRail();
	UFUNCTION(CallInEditor, Category = "Rails") void SpawnReturnRail();
	//UFUNCTION(CallInEditor, Category = "Rails") void SpawnChangeWayReturnRail();

	void UpdateRailIsBlocked(const bool _incrementIndex, const bool _updateIndex = true, const bool _clamp0 = true);
	void AddRail(ARailActor* _rail, const bool _isDepartureRail);
	ARailActor* GetNextRail(const bool _isOnReturn, AStopHandcarActor* _nextStop = nullptr);

	void CheckRailsOnHandcarPath(ACustomHandcarActor* _handcar);



	UFUNCTION() void OnRep_UpdateIndexReturn(const int _index);
	UFUNCTION() void OnRep_UpdateIndexDeparture(const int _index);
};
