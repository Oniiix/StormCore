// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Flower.h"
#include <Components/SphereComponent.h>
#include "ZoneFlower.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ROBOTHUNTER_API AZoneFlower : public AFlower
{
	GENERATED_BODY()
	
#pragma region TimerSetting
protected:
	/*Duree entre chaque hit de degat*/
	UPROPERTY(EditAnywhere, Category = "ZoneFlower|Timer Settings")	float hitRate = 1.f;
	UPROPERTY(EditAnywhere, Category = "ZoneFlower|Timer Settings")	bool activeDamageDuringTime = true;
#pragma endregion
#pragma region Random
protected:
	UPROPERTY(EditAnywhere, Category="ZoneFlower|Random") bool isRandomEffect = false;
	UPROPERTY(EditAnywhere, Category="ZoneFlower|Random", meta=(EditCondition = "isRandomEffect", EditConditionHides))int effectMax = 10;
	UPROPERTY(EditAnywhere, Category="ZoneFlower|Random", meta=(EditCondition = "isRandomEffect", EditConditionHides))int effectMin = 0;
	UPROPERTY(EditAnywhere, Category = "ZoneFlower|Random")bool isRandomCooldown = false;
	UPROPERTY(EditAnywhere, Category = "ZoneFlower|Random", meta = (EditCondition = "isRandomCooldown", EditConditionHides))int cooldownMax = 10;
	UPROPERTY(EditAnywhere, Category = "ZoneFlower|Random", meta = (EditCondition = "isRandomCooldown", EditConditionHides))int cooldownMin = 0;
#pragma endregion
protected:
	//Use for different interaction with player (box, shpere, capsule..)
	UPROPERTY(VisibleAnywhere) TObjectPtr<UShapeComponent> areaEffect;
	UPROPERTY(VisibleAnywhere)TArray<AActor*> actorWithEffect = TArray<AActor*>();
private:
	bool inOverlap = false;

#pragma region Timer
protected:
	FTimerHandle zoneTimer;
	FTimerHandle timerFlowerPhase;
#pragma endregion


#pragma region Methods
protected:
	virtual void Init() override;
	/// <summary>
	/// Function that detects returning actors and saves them in a list to apply the effects during plant activation
	/// </summary>
	UFUNCTION() void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// <summary>
	/// Execute when a actor exit the area of box collider. 
	/// </summary>
	/// <param name="OtherActor">actor detected</param>
	UFUNCTION() void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void ActivePhase() override;
	virtual void CooldownPhase() override;
	/// <summary>
	/// Initializes the timer which loops by calling the Zone Effect function
	/// </summary>
	/// <param name="_actorWithEffect">Actor who entered the area of effect</param>
	void InitZoneTimer(TArray<AActor*> _actorWithEffect);
	/// <summary>
	/// Effect Applied on a actor when 
	/// </summary>
	/// <param name="_actor"></param>
	virtual void ZoneEffect(TArray<AActor*> _actorWithEffect) {}
	virtual void ExitZoneEffect(AActor* _actorExitZoneEffect) {}
	/// <summary>
	/// Clear Timer of zoneTimer.
	/// </summary>
	void ClearTimer();
#pragma region RPC
	//UFUNCTION(NetMulticast,Reliable) void MultiRpc_Cooldown();
#pragma endregion

};
