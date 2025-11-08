// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageFlower.h"
#include "StormFlower.generated.h"

class APowerLine;
UCLASS()
class ROBOTHUNTER_API AStormFlower : public ADamageFlower
{
	GENERATED_BODY()
#pragma region Property
	UPROPERTY(EditAnywhere, Category = "StormFlower|Property")	TSubclassOf<APowerLine> spineLine = nullptr;
	UPROPERTY(EditAnywhere, ReplicatedUsing = On_RepFlowerLink, Category = "StormFlower|Property")TArray<AStormFlower*> flowerLink = TArray<AStormFlower*>();
	UPROPERTY(EditAnywhere, Category = "StormFlower|Property")TArray<TEnumAsByte<EObjectTypeQuery>> layerFlower = TArray<TEnumAsByte<EObjectTypeQuery>>();
	UPROPERTY(EditAnywhere, Category = "StormFlower|Property", meta = (ClampMin = 0.1f))float modifierRadius = 2;
	UPROPERTY(ReplicatedUsing = OnRep_SphereRadius)float sphereRadius = 2;
	UPROPERTY(EditAnywhere, Category = "StormFlower|Property", meta = (ClampMin = 0.1f))float modifierDamage = 2;
	/// <summary>
	/// Cooldown between each flower charging each other
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "StormFlower|Timer Settings")float cooldownFlowerAlim = 4.f;

	bool isCooldownFlowerPhase = false;
	
	FVector directionTwoFlower = FVector();

	float oldDamage;
	float oldSphereRadius;
#pragma endregion
public:
	TArray<AStormFlower*> GetFlowerLink()const {return flowerLink;}
public:

	AStormFlower();
#pragma region Methods
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void Init() override;
	virtual void DrawDebug() override;
public:
	/// <summary>
/// Time between each deactivation of the flower by another flower (time and the same as with lightning)
/// </summary>
	void ActiveFlowerPhase();
private:
	
	void RemoveFlowerLink();
	/// <summary>
	/// Time before a plant can activate this
	/// </summary>
	void CooldownFlowerPhase();
	/// <summary>
	/// Set the isCooldownFlowerPhase boolean to false to inform that it is not in cooldown
	/// </summary>
	void EndCooldownFlower();
	virtual void ZoneEffect(TArray<AActor*> _actorWithEffect) override;
	/// <summary>
	/// Spawn the power line between the flowers
	/// </summary>
	void SpawnPowerLine();
	/// <summary>
	/// Initializes during spawning the rotation of the power line based on two flowers
	/// </summary>
	/// <param name="_from">First flower location</param>
	/// <param name="_to">Second flower location</param>
	/// <returns></returns>
	FRotator InitSpawnRotation(const FVector& _from, const FVector& _to);
	/// <summary>
	/// Initializes during spawning the Location of the power line based on two flowers
	/// </summary>
	/// <param name="_a">First flower</param>
	/// <param name="_b">Second flower</param>
	/// <returns></returns>
	FVector InitSpawnLocation(const FVector& _a, const FVector& _b);
	void CheckFlower(AStormFlower* _flower);
	void CheckLink(AStormFlower* _flower);
	UFUNCTION(Server, Unreliable, WithValidation) void ServerRPC_SpawnPowerLine();
	UFUNCTION() void On_RepFlowerLink();
	UFUNCTION() void OnRep_SphereRadius();
	void ActivateModifier();
	void DesactivateModifier();
public:
	bool AsFlowerAlreadyLink(AStormFlower* _flower);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerActivateModifier();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDesactivateModifier();
#pragma endregion

};
