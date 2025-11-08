

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "STTask_MoveFlankInRange.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_MoveFlankInRange : public USTTask_Movement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float offSetShoot = 200;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AMainCharacter> player = nullptr;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERangeAttack> range;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

};
