// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../HUBInteractableActor.h"
#include "../../../Managers/Day/DayManager.h"
#include "StartInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AStartInteractableActor : public AHUBInteractableActor
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		TObjectPtr<ADayManager> dayManager = nullptr;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Interacted(AMainCharacter* _character) override;

	virtual void Interacted_Internal() override;
private:
	UFUNCTION() virtual void ShowMenu() override;
	
};
