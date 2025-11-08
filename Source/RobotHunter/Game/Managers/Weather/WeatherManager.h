// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../CustomActor.h"
#include <RobotHunter/Game/GPE/Weathers/Weather.h>
#include "../../Interactable/Hub/RadioActor/RadioActor.h"
#include "../Day/DayManager.h"
#include "WeatherManager.generated.h"


USTRUCT()
struct FWeatherForTheDay
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AWeather>> dayWeathers;

};

#define WEATHER_MANAGER_USE_LOG false
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AWeatherManager : public ACustomActor
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	int weatherIndex = 0;
	UPROPERTY(Replicated)
	TSubclassOf<AWeather> chosenWeather = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherManager|Property", meta = (AllowPrivateAccess))
	TArray<TSubclassOf<AWeather>> allWeathers = {};

	UPROPERTY(EditAnywhere, Category = "WeatherManager|Property")
	TObjectPtr<ARadioActor> radio = nullptr;
	UPROPERTY(EditAnywhere, Category = "WeatherManager|Property")
	bool onlyFirstWeather = false;
#pragma region custom days
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADayManager> dayManager = nullptr;


	UPROPERTY(VisibleAnywhere, Category = "WeatherManager|Custom Days")
	TArray<FWeatherForTheDay> allDayWeathers = {};
	UPROPERTY(EditAnywhere, Category = "WeatherManager|Custom Days")
	FWeatherForTheDay firstDayWeathers = {};
	UPROPERTY(EditAnywhere, Category = "WeatherManager|Custom Days")
	FWeatherForTheDay secondDayWeathers = {};
	UPROPERTY(EditAnywhere, Category = "WeatherManager|Custom Days")
	FWeatherForTheDay thirdDayWeathers = {};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AMainCharacter> chara = nullptr;


#pragma endregion

public:
	// Sets default values for this actor's properties
	AWeatherManager();
	virtual void BeginPlay() override;
	void InitAllDayWeathers();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetDayManager(ADayManager* _dayManager) { dayManager = _dayManager; }
	FORCEINLINE void SetWeatherIndex(float _weatherIndex) { weatherIndex = _weatherIndex; }
	FORCEINLINE float GetWeatherIndex() { return weatherIndex; }

public:

	UFUNCTION()
	void SelectWeather();
	UFUNCTION(Server, Reliable)
	void ServerRpc_SetClientWeather(TSubclassOf<AWeather> _weather);
	void SetWeather();
	UFUNCTION(Server, Unreliable)
	void ServerRpc_SetWeather(float _index);
	void OnRep_SetWeather(float _index);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void InitCharacter(AMainCharacter* _chara);
};
