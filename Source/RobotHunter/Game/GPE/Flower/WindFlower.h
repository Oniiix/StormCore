// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZoneFlower.h"
#include "WindFlower.generated.h"



UCLASS()
class ROBOTHUNTER_API AWindFlower : public AZoneFlower
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "AWindFlower", meta = (ClampMin = 0.1f))float windForceForward = 1000;
	UPROPERTY(EditAnywhere, Category = "AWindFlower", meta = (ClampMin = 0.1f)) float windForceUp = 1000;
	UPROPERTY(EditAnywhere, Category = "AWindFlower", meta = (ClampMin = 0.1f))float modifierWindForceForward = 3.0f;
	UPROPERTY(EditAnywhere, Category = "AWindFlower") float modifierWindForceUp = 3.0f;

	bool activateModifier = false;
public:
	AWindFlower();
protected:
	virtual void Init() override;
	UFUNCTION()void ActiveModifier();
	UFUNCTION()void DesactivateModifier();
	virtual void ZoneEffect(TArray<AActor*> _actorWithEffect) override;
};
