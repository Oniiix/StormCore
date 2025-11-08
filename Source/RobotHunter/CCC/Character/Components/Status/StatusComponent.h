#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActorComponent.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "StatusComponent.generated.h"

USTRUCT()
struct FStatusBySource
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TMap<UClass*, UStatus*> statusBySource = TMap<UClass*, UStatus*>();

	FStatusBySource() = default;
	FStatusBySource(UClass* _class, UStatus* _status)
	{
		if (!_class || !_status)
			return;

		statusBySource.Add(_class, _status);
	}
};

UCLASS()
class ROBOTHUNTER_API UStatusComponent : public UCustomActorComponent
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusTypeAdded, EStatusType, type);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusTypeRemoved, EStatusType, type);

	UPROPERTY(VisibleAnywhere)
	TMap<TEnumAsByte<EStatusType>, FStatusBySource> currentStatus;

	UPROPERTY(VisibleAnywhere)
	TMap<UClass*, UStatus*> currentResistance;

	UPROPERTY()
	TObjectPtr<UStatus> statusToAdd;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStatusTypeAdded onStatusTypeAdded;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStatusTypeRemoved onStatusTypeRemoved;

	float resistanceBonus;

public:
	FORCEINLINE void SetResistanceBonus(const float _bonus) { resistanceBonus = _bonus; }

public:
	UStatusComponent();

private:
	void AddStatusOfNewType();
	void AddStatusOfExistingType();

	void RefreshStatusAppliedByType(const TMap<UClass*, UStatus*>& _statusBySource, const EStatusType& _type, 
		const EStatusType& _resistanceType, UStatus* _currentAppliedStatus = nullptr);
	bool CheckIfResists(const EStatusType& _newStatusType);

	UStatus* CreateStatusFromType(AActor* _source, const FStatusParams& _params);

	template<typename StatusType>
	StatusType* CreateStatus(AActor* _source, const FStatusParams& _params);

public:
	void RefreshStatus(AActor* _source, const FStatusParams& _params, const bool _sourceCooldownPhase);
	void RemoveStatus(AActor* _source, UStatus* _status);
	void RemoveStatus(AActor* _source, const EStatusType& _statusType);

	void AddResistance(UClass* _source, UStatus* _resistance);
	void RemoveResistance(UClass* _source);

	float GetStatusTypeMaxValue(const EStatusType& _type, const EStatusType& _resistanceType);
	UStatus* AddStatus(AActor* _source, const FStatusParams& _params, const bool _delayApplication = false);
};

template<typename StatusType>
inline StatusType* UStatusComponent::CreateStatus(AActor* _source, const FStatusParams& _params)
{
	if (!_source)
		return nullptr;

	StatusType* _status = NewObject<StatusType>(this);
	_status->SetParams(_params);
	_status->SetStatusComponent(this);
	_status->SetSource(_source);
	return _status;
}