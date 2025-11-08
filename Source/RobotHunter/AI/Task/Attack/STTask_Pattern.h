#pragma once

#include "CoreMinimal.h"
#include "../STTask.h"
#include "STTask_Pattern.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_Pattern : public USTTask
{
	GENERATED_BODY()

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
