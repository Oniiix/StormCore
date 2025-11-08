

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/Component/ArenaCombat/ArenaCombatComponent.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "STCondition_TestAttackProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTCondition_TestAttackProjectile : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERangeAttack> range = ERangeAttack::Long;

protected:
	bool TestCondition(FStateTreeExecutionContext& Context) const override;
};
