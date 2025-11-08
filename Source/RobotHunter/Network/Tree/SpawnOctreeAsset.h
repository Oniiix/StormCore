// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnTreeDataAsset.h"
#include "SpawnOctreeAsset.generated.h"

#define SPAWN_OCTREE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API USpawnOctreeAsset : public USpawnTreeDataAsset
{
	GENERATED_BODY()

public:
	virtual void Spawn(ATreeCell* _parent) override;
	
};
