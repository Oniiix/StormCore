// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LootActor.h"
#include "CarcassActor.generated.h"

#define CARCASS_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API ACarcassActor : public ALootActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int numberLoot = 0;
	UPROPERTY(EditAnywhere)
	int maxDrop = 4;
	UPROPERTY(EditAnywhere)
	int minDrop = 3;

public:
	virtual void Interacted(AMainCharacter* _character)override;
};
