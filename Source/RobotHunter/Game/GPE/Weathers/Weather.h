// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../CustomActor.h"
#include <NiagaraSystem.h>
#include <RobotHunter/Utils/ENUM_Meteo.h>
#include "Weather.generated.h"

#define WEATHER_USE_LOG true

USTRUCT(Blueprintable)
struct FWeatherStats
{
	GENERATED_BODY()

#pragma region EarthQuake
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100), Category = "EarthQuake")
	int earthQuakePercent = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess), Category = "EarthQuake")
	TObjectPtr<UNiagaraSystem> earthNiagara;
	USceneComponent* earthNiagaraSceneComp;
	class UNiagaraComponent* earthNiagaraComp;

	UPROPERTY(EditAnywhere, Category = "EarthQuake", meta = (ForceUnits = "cm/s"))
	float maxEarthQuakeIntensity = 5;
	UPROPERTY(EditAnywhere, Category = "EarthQuake", meta = (ForceUnits = "cm/s"))
	float minEarthQuakeIntensity = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "EarthQuake", meta = (ForceUnits = "cm/s"))
	float currentEarthQuakeIntensity = 0;

#pragma endregion


#pragma region Wind
	UPROPERTY(EditAnywhere, Category = "Wind", meta = (ForceUnits = "cm/s"))
	float maxWindIntensity = 5;
	UPROPERTY(EditAnywhere, Category = "Wind", meta = (ForceUnits = "cm/s"))
	float minWindIntensity = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess),Category = "Wind")
	FVector windDirection = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "Wind", meta = (ForceUnits = "cm/s"))
	float currentWindIntensity = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess), Category = "Wind")
	TObjectPtr<UNiagaraSystem> windNiagara;
	USceneComponent* windNiagaraSceneComp;
	class UNiagaraComponent* windNiagaraComp;

#pragma endregion




	FORCEINLINE int GetEarthQuakeStat() const { return earthQuakePercent; }
	FORCEINLINE float GetCurrentIntensity() const { return currentWindIntensity; }
	FORCEINLINE TObjectPtr<UNiagaraSystem> GetEarthNiagara() const { return earthNiagara; }
	FORCEINLINE TObjectPtr<UNiagaraSystem> GetWindNiagara() const { return windNiagara; }

};

UCLASS()
class ROBOTHUNTER_API AWeather : public ACustomActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSubLevelLoad);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWindChanged, FVector, _windDirection, float, _windIntensity);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEarthQuake, float, _earthQuakeIntensity);
	
	UPROPERTY(EditAnywhere, Category = "Weather")
	TArray<TSoftObjectPtr<UWorld>> levelsToLoad;

	UPROPERTY(EditAnywhere, Category = "Weather")
	FString name = "";
	UPROPERTY(EditAnywhere, Category = "Weather")
	FWeatherStats stats;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weather", meta = (AllowPrivateAccess))
	TObjectPtr<UNiagaraSystem> niagara;
	UPROPERTY(EditAnywhere, Category = "Weather")
	TEnumAsByte<EMeteoStatus> meteoType;

	int index;

	bool hasEarthQuake = false;


	UPROPERTY()
	FOnSubLevelLoad onSubLevelLoad;

	UPROPERTY(EditAnywhere, Category = "Weather")
	USceneComponent* niagaraSceneComp;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	class UNiagaraComponent* niagaraComp;
	
	UPROPERTY(EditAnywhere, Category = "Weather", BlueprintAssignable,BlueprintCallable)
	FOnWindChanged onWindChanged;
	UPROPERTY(EditAnywhere, Category = "Weather", BlueprintAssignable,BlueprintCallable)
	FOnEarthQuake onEarthQuake;

public:
	FORCEINLINE FString GetWeatherName() const { return name; }
	FORCEINLINE TEnumAsByte<EMeteoStatus> GetMeteoType() const { return meteoType; }
	FORCEINLINE FOnWindChanged& OnWindChanged() { return onWindChanged; }
	FORCEINLINE FOnEarthQuake& OnEarthQuake() { return onEarthQuake; }
	FORCEINLINE FWeatherStats GetWeatherStats() const { return stats; }
	UFUNCTION(BlueprintCallable) FORCEINLINE TArray<TSoftObjectPtr<UWorld>> GetLevels() const { return levelsToLoad; }

public:
	AWeather();
	virtual void BeginPlay() override;
	void InitWorld();
	UFUNCTION()void InitMeteo();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() virtual void LoadAllLevels();


	void LoadLevel(TSoftObjectPtr<UWorld> _level);
	void UnloadLevel(TSoftObjectPtr<UWorld> _level);

	UFUNCTION() void LoadLevelCallback();

	void InitEarthQuake();
	void InitWind();
	void UpdateWind();
	void SpawnNiagara(TObjectPtr<UNiagaraSystem> _niagaraSystem,USceneComponent* _sceneComponent,UNiagaraComponent* _niagaraComponent);
	FVector SelectDirection();
	
	void LoadTree();

};
