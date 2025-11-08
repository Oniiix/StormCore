// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/InteractableActor.h"
#include "DummyButton.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ADummyButton : public AInteractableActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Dummy")
	TArray<AActor*> dummyTargets;


	TArray<FVector> initPositions;

public:
	virtual void Interacted(AMainCharacter* _character) override;

protected:
	virtual void BeginPlay() override;
};
