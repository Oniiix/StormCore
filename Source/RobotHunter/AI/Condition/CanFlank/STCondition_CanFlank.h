

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STCondition_CanFlank.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTCondition_CanFlank : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int percentageRand = 50;

protected:
	 bool TestCondition(FStateTreeExecutionContext& Context) const override;
};
