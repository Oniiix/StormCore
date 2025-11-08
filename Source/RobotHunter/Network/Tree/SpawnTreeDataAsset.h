// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnTreeDataAsset.generated.h"


class ATreeCell;

UCLASS(Abstract)
class ROBOTHUNTER_API USpawnTreeDataAsset : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATreeCell> childTreeRef = nullptr;

public:
	virtual void Spawn(ATreeCell* _parent);

};
