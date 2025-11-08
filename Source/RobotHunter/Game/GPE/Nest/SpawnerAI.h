// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerAI.generated.h"

#define SPAWNER_USE_LOG false
#define MIN_SPAWN_DURATION 2.f

class AAI_Base;
class AArena;
class UParalarvaSettingsDataAsset;
class UTitanSettingsDataAsset;
class UAIsSettingsDataAsset;
enum EAIType;

UCLASS()
class ROBOTHUNTER_API ASpawnerAI : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSpawning);
	FOnEndSpawning onEndSpawning;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<UStaticMeshComponent> mesh = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<USceneComponent> spawnPoint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<AArena> arena = nullptr;
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (UMin = 2.f, ClampMin = 2.f))
	float spawnRate = 2.f;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TArray<TSubclassOf<AAI_Base>> listAIsRef;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<UParalarvaSettingsDataAsset> paralarvaSettings = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<UTitanSettingsDataAsset> titanSettings = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TArray<TEnumAsByte<EAIType>> listAIsToSpawn;

	FTimerHandle timerSpawn;
	bool spawning = false;
	float duration = 0.f;
	FVector spawnLocation;
	FRotator spawnRotation;
	bool isInitializationGood = false;
	
public:	
	ASpawnerAI();

	FORCEINLINE FOnEndSpawning& OnEndSpawning() { return onEndSpawning; }
	FORCEINLINE FVector GetSpawnLocation() { return spawnLocation; }
	FORCEINLINE FRotator GetSpawnRotation() { return spawnRotation; }

	FORCEINLINE void SetVisibility(const bool _value) { RootComponent->SetVisibility(_value); }

	/// <summary>
	/// Init spawner
	/// </summary>
	/// <param name="_arena"> The arena which spawner is attached </param>
	void InitSpawnerAI(AArena* _arena);

	/// <summary>
	/// Spawn an AI 
	/// </summary>
	/// <param name="_AIRef"></param>
	void SpawnAI(TSubclassOf<AAI_Base> _AIRef, TObjectPtr<UAIsSettingsDataAsset> _AISettings);

	UFUNCTION()
	void AddAIToSpawn(const EAIType& _type);

private:
	void Tick(float DeltaSeconds) override;
};
