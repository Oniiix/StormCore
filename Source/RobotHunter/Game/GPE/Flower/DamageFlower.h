// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZoneFlower.h"
#include "DamageFlower.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ADamageFlower : public AZoneFlower
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Replicated, Category = "DamageFlower")float damage = 10;

#pragma region Methods
protected:
	void AppliedDamage(TArray<AActor*> _actorWithEffect);
	virtual void ZoneEffect(TArray<AActor*> _actorWithEffect) override;
#pragma endregion

};
