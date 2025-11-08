// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayManager.generated.h"

#define DAY_MANAGER_USE_LOG false

UCLASS()
class ROBOTHUNTER_API ADayManager : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
		int dayCounter = 1;
	
public:	
	ADayManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE int GetCurrentDay() const { return dayCounter; }
public:

	void UpdateDay();
	void LoadActualDay(int _actualDay);

};
