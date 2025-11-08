#pragma once

#include "CoreMinimal.h"
#include "../InteractableActor.h"
#include "../../../Splines/Rail/RailActor.h"
#include "../../../Splines/Rail/ChangeWayRailActor.h"
#include "LeverActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ALeverActor : public AInteractableActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateRailIsBlocked);

	UPROPERTY(EditAnywhere, Category = "Rails")
	TSubclassOf<ARailActor> railType;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TSubclassOf<AChangeWayRailActor> changeWayRailType;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TArray<ARailActor*> returnRails;

	UPROPERTY(EditAnywhere, Category = "Rails")
	TArray<ARailActor*> departureRails;

	/*UPROPERTY(VisibleAnywhere)
	bool isReverse;*/

	int currentIndexReturn;
	int currentIndexDeparture;

	bool isLinkedToHandcar;

	FOnUpdateRailIsBlocked onUpdateRailIsBlocked;

public:
	/// <summary>
	/// Set to true if the lever's bifurcations are towards the ending point (return direction).
	/// </summary>
	/// <param name="_value"></param>
	//FORCEINLINE void SetIsReverse(const bool _value) { isReverse = _value; }
	FORCEINLINE void SetIsLinkedToHandcar(const bool _isLinkedToHandcar) { isLinkedToHandcar = _isLinkedToHandcar; }

	FORCEINLINE bool GetIsLinkedToHandcar() const { return isLinkedToHandcar; }
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
	ALeverActor();

private:
	void DrawDebugRails(const TArray<ARailActor*>& _rails, const int _currentIndex, const bool _useDepartureStop);

	void UpdateCurrentIndex(TArray<ARailActor*> _rails, int& _currentIndex);
	void UpdateRailIsBlocked_Internal(TArray<ARailActor*>& _rails, int& _currentIndex, const bool _updateIndex = true);
	void UpdateRailIsBlocked(const bool _updateIndex = true);

	void RefreshRails(TArray<ARailActor*>& _toRefreshRails);

	void SpawnRail_Internal(const bool _isDepartureRail);
	void SpawnChangeWayRail_Internal(const bool _isDepartureRail);

protected:
	virtual void BeginPlay() override;
	virtual void DrawDebug() override;

public:
	UFUNCTION(CallInEditor, Category = "Interact") virtual void Interacted(AMainCharacter* _character) override;
	UFUNCTION(CallInEditor, Category = "Rails") void SpawnDepartureRail();
	//UFUNCTION(CallInEditor, Category = "Rails") void SpawnChangeWayDepartureRail();
	UFUNCTION(CallInEditor, Category = "Rails") void SpawnReturnRail();
	//UFUNCTION(CallInEditor, Category = "Rails") void SpawnChangeWayReturnRail();

	void AddRail(ARailActor* _rail, const bool _isDepartureRail);
	ARailActor* GetNextRail(const bool _isOnReturn) const;
};
