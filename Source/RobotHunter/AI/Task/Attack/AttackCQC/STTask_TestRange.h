#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/STTask.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "STTask_TestRange.generated.h"

UCLASS()
class ROBOTHUNTER_API USTTask_TestRange : public USTTask
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERangeAttack> rangeToTest = ERangeAttack::RangeAttack_Count;

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
