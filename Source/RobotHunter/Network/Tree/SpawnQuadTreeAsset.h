

#pragma once

#include "CoreMinimal.h"
#include "SpawnTreeDataAsset.h"
#include "SpawnQuadTreeAsset.generated.h"

#define SPAWN_QUAD_USE_LOG false


UCLASS()
class ROBOTHUNTER_API USpawnQuadTreeAsset : public USpawnTreeDataAsset
{
	GENERATED_BODY()

public:
	virtual void Spawn(ATreeCell* _parent) override;
};
