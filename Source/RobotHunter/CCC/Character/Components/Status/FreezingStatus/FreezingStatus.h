#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/StackingStatus.h"
#include "FreezingStatus.generated.h"

#define FREEZING_STATUS_USE_LOG false

class AMainCharacter;
class AAI_Base;

UCLASS()
class ROBOTHUNTER_API UFreezingStatus : public UStackingStatus
{
	GENERATED_BODY()

	bool useMuliplierWhenAddingStack;
	float stackDurationMultiplier;

	bool hasSpeedModifier;
	float speedModifier;

	FStatusParams stunnedStatus;
	FStatusParams protectedStatus;
	FStatusParams resistantStatus;

public:
	FORCEINLINE void SetStackDurationMultiplier(const bool _useMuliplierWhenAddingStack, const float _stackDurationMultiplier)
	{
		useMuliplierWhenAddingStack = _useMuliplierWhenAddingStack;
		stackDurationMultiplier = _stackDurationMultiplier;
	}
	FORCEINLINE void SetMaxStackStatus(const FStatusParams& _stunnedStatus, const FStatusParams& _protectedStatus, const FStatusParams& _resistantStatus)
	{
		stunnedStatus = _stunnedStatus;
		protectedStatus = _protectedStatus;
		resistantStatus = _resistantStatus;
	}
	
public:
	UFreezingStatus();
	UFreezingStatus(const FStatusParams& _params);

private:
	void SlowAI(AAI_Base* _ai);

protected:
	virtual void StackBehaviour() override;
	virtual void MaxStackBehaviour() override;

public:
	virtual void ApplyStatus(AActor* _actor) override;
	virtual void UnapplyStatus(AActor* _actor) override;
	virtual void StartStatusTimer(const float _duration = -1.0f) override;
	virtual void RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor = nullptr) override;
	virtual float GetValue() const override;
};
