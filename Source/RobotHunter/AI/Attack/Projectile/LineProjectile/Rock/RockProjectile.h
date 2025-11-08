// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LineProjectile.h"
#include "RockProjectile.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API ARockProjectile : public ALineProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock")
	float roadDistance = 600;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock|Size")
	float initSize = 1;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock|Size")
	float maxSize;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock|Size")
	float timeSize = 3;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock|Damage")
	float initDamage;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock|Damage")
	float maxDamage;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Rock")
	float speedSize;

	float currentTime = 0;

	FVector newScale, maxScale,initScale;

protected:
	virtual void InitProjectile(const FVector& _targetLocation) override;

protected:
	virtual void Tick(float _deltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	void SizeProjectile();
	float GetDamage();
};
