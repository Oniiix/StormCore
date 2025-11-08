// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

#define TIME_MANAGER_USE_LOG false

class UNetworkGPEComponent;

UCLASS()
class ROBOTHUNTER_API ATimeManager : public AActor
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateTime, const int, _hour, const int, _minute);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewHourStart, const int, _hour);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndDay);
	GENERATED_BODY()

	/// Hours
	UPROPERTY(EditAnywhere)
	float dayTimeValue = 12;

	/// <summary>
	/// Minutes
	/// </summary>
	UPROPERTY(EditAnywhere)
	float inGameDayDuration = 20;

	FOnUpdateTime onUpdateTime;

	FOnNewHourStart onNewHourStart;

	FOnEndDay onEndDay;

	float inGameHour = 0;
	float inGameMinutes = 0;

	UPROPERTY(EditAnywhere)
	float startHour = 8;

	TObjectPtr< UNetworkGPEComponent> net = nullptr;

	FTimerHandle dayTimer;

	bool hasAuthority = false;
public:	
	ATimeManager();
	FORCEINLINE FOnUpdateTime& OnUpdateTime() { return onUpdateTime; }
	FORCEINLINE FOnNewHourStart& OnNewHourStart() { return onNewHourStart; }
	FORCEINLINE FOnEndDay& OnEndDay() { return onEndDay; }
	FORCEINLINE float GetInGameHour()const { return inGameHour; }
	FORCEINLINE float GetInGameMinutes()const { return inGameMinutes; }
	FORCEINLINE float GetRealHour()const { return inGameHour + startHour; }

	/**
	 * Return the duration real in minutes
	 * @return Real duration
	 */
	FORCEINLINE float GetInGameDuration()const { return inGameDayDuration; }
	
	/**
	 * Return the game minutes total
	 * @return 
	 */
	FORCEINLINE float GetInGameDurationInMinutes()const { return dayTimeValue * 60; }
	
	/**
	 * Return the game minutes elapsed
	 * @return 
	 */
	FORCEINLINE float GetInGameElapsedInMinutes()const { return (inGameHour * 60) + inGameMinutes; }

	FORCEINLINE void SetMinute(const float _currentMinute)
	{
		inGameMinutes = _currentMinute;
	}
	FORCEINLINE void SetHour(const float _currentHour)
	{
		inGameHour = _currentHour;
	}


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:	
	void UpdateHour();

	void SetupDay();

	void UpdateMinute();
};
