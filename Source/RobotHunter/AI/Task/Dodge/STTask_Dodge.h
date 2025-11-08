

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "STTask_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_Dodge : public USTTask_Movement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> player = nullptr;
	UPROPERTY(EditAnywhere)
	float lengthDash = 100;
	UPROPERTY(EditAnywhere)
	float speedDodge = 100;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte< EObjectTypeQuery>> layerDodge;

	FVector targetPosition;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime)override;


};
