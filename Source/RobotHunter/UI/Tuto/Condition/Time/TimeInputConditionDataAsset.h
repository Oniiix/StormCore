

#pragma once

#include "CoreMinimal.h"
#include "../ConditionDataAsset.h"
#include "InputMappingContext.h"
#include "TimeInputConditionDataAsset.generated.h"

#define TIME_INPUT_CONDITION_USE_LOG false


UCLASS(BlueprintType)
class ROBOTHUNTER_API UTimeInputConditionDataAsset : public UConditionDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> inputAction = nullptr;

	UPROPERTY(EditAnywhere, meta = (UMin = 0, ClampMin = 0))
	float pressedTime = 2.0f;

	float currentPressedTime = 0.0f;

	int handle = -1;


public:
	virtual void InitCondition(AMainCharacter* _chara) override;
	virtual void CancelCondition() override;

private:
	UFUNCTION() void InputPressed(const FInputActionValue& _value);
};
