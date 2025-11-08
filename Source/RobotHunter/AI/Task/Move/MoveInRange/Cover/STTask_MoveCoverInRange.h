#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Task/Move/STTask_Movement.h"
#include "RobotHunter/Game/GPE/Components/Cover/CoverComponent.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "STTask_MoveCoverInRange.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USTTask_MoveCoverInRange : public USTTask_Movement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TEnumAsByte<ERangeAttack> typeRange = ERangeAttack::Long;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<AMainCharacter> player = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TArray<UCoverComponent*> listCover;
	UPROPERTY(EditAnywhere, Category = "Output", BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool forceFlank = false;
	TObjectPtr<AAI_Hunter> hunter = nullptr;

protected:
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

private:
	UCoverComponent* GetCover(const TArray< UCoverComponent*> _listCover, const FVector& _actorLocation);
};
