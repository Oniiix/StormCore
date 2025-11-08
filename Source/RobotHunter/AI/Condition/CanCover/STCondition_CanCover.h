

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "STCondition_CanCover.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTCondition_CanCover : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERangeAttack> typeRange;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AMainCharacter> player;

protected:
	bool TestCondition(FStateTreeExecutionContext& Context) const override;

};
