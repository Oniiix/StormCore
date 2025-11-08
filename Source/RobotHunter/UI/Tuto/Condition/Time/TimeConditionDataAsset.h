

#pragma once

#include "CoreMinimal.h"
#include "../ConditionDataAsset.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "TimeConditionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTimeConditionDataAsset : public UConditionDataAsset, public FTickableGameObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float duration = 5.0f;

	float currentTime = 0.0f;

	bool isRunning = false;

public:
	virtual void InitCondition(AMainCharacter* _chara) override;
	virtual void CancelCondition() override;

	void Tick(float DeltaTime) override;
	bool IsTickable() const override { return isRunning; }
	bool IsTickableInEditor() const override { return false; }
	bool IsTickableWhenPaused() const override { return false; }
	TStatId GetStatId() const override { return TStatId(); }
};
