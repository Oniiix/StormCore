#pragma once

#include "Prefab.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "OptionablePrefab.generated.h"

class UOptionData;

UCLASS(Abstract)
class ROBOTHUNTER_API UOptionablePrefab : public UPrefab
{
	GENERATED_BODY()


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDetected, const UOptionablePrefab*, _prefab);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeIndex, const int&, _index);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHover, const FString&, _msg);

	UPROPERTY()
	FOnHover onHover;

	UPROPERTY()
	FOnChangeDetected onChangeDetected;

	UPROPERTY()
	FOnChangeIndex onChangeIndex;

protected:
	UPROPERTY()
	TObjectPtr<UOptionData> data;

	UPROPERTY(VisibleAnywhere)
	int index = 0;

public:
	FORCEINLINE FOnChangeDetected& OnChangeDetected() { return onChangeDetected; }
	FORCEINLINE FOnChangeIndex& OnChangeIndex() { return onChangeIndex; }
	FORCEINLINE FOnHover& OnHover() { return onHover; }
	FORCEINLINE virtual void SetCurrentIndex(const int& _index) { index = _index; data->SetHasChanged(true); }
	FORCEINLINE virtual void SetCurrentIndexWithoutEvent(const int& _index)
	{
		index = _index;
		data->ChangeValue(_index);
	}

	FORCEINLINE void Callback_OnChangeDetected() const { onChangeDetected.Broadcast(this); }
	FORCEINLINE void Callback_OnChangeIndex(const int& _index) const { onChangeIndex.Broadcast(_index); }
	FORCEINLINE int GetCurrentIndex() const { return index; }
	FORCEINLINE TObjectPtr<UOptionData> GetData() const { return data; }

	virtual void SetOptionData(const TObjectPtr<UOptionData>& OptionData);

private:
	UFUNCTION()
	void SetChangeDetected(const int& I);
	/**
	 * Set the option data in prefab.
	 * @param _name Name of option
	 * @param _list List of FText of option. (Selector = List FText, Slider = FText of value min and max) 
	 */
	virtual void SetOptionParameter(const FString& _name, const TArray<FString>& _list);

	/**
	 * Callback event, to show description of data if data available
	 */
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess))
	FORCEINLINE void Callback_OnHover() { onHover.Broadcast(data ? data->GetDescription() : ""); }

	/**
	 * Callback event, to hide description
	 */
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess))
	FORCEINLINE void Callback_OnUnHover() { onHover.Broadcast(""); }
};
