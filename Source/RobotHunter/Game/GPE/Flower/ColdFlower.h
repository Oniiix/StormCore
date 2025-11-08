// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageFlower.h"
#include "ColdFlower.generated.h"



UCLASS()
class ROBOTHUNTER_API AColdFlower : public ADamageFlower
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Property", meta = (ClampMin = 0.1f)) float frostDuration = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Property", meta = (ClampMin = 0.1f)) float modifierFrostDuration = 5.0f;

	float currentFrostDuration = 0.0f;
	FTimerHandle timerFrostDuration;
public:
	AColdFlower();
protected:
	virtual void Init() override;
	virtual void ZoneEffect(TArray<AActor*> _actorWithEffect) override;
	void ExitZoneEffect(AActor* _actorExitZoneEffect) override;
private:
	UFUNCTION()void ApplyModifierFrostDuration();
	UFUNCTION()void RemoveModifierFrostDuration();
	UFUNCTION()void RemoveFrostDuration(AMainCharacter* _character);
};
