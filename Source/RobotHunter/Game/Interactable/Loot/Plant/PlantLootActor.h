#pragma once

#include "CoreMinimal.h"
#include "../LootActor.h"
#include "PlantLootActor.generated.h"

#define PLANTLOOTACTOR_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API APlantLootActor : public ALootActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool isRandom = true;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "!isRandom", EditConditionHides))
	int numberLoot = 0;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "isRandom", EditConditionHides))
	int maxDrop = 1;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "isRandom", EditConditionHides))
	int minDrop = 3;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float givenLife = 10.0f;


public:
	virtual void Interacted(AMainCharacter* _character)override;
};
