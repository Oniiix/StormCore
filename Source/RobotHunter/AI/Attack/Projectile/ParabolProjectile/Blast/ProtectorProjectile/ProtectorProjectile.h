// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BlastProjectile.h"
#include "ProtectorProjectile.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API AProtectorProjectile : public ABlastProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Blast")
	float timeChargeProjectile = 2;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Blast")
	float speedChargeProjectile = 100;

	float currentTime = 0;

public:
	virtual void InitProjectile(const FVector& _targetLocation) override;

protected:
	virtual void Tick(float _deltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SpawnTriggerSphere();

private:
	void GrowProjectile();
};
