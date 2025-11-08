#pragma once

#include "CoreMinimal.h"
#include "../ConditionDataAsset.h"
#include "InputMappingContext.h"
#include "SimpleInputConditionDataAsset.generated.h"

#define SIMPLE_INPUT_CONDITION_USE_LOG true


UCLASS(BlueprintType)
class ROBOTHUNTER_API USimpleInputConditionDataAsset : public UConditionDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> inputAction = nullptr;

	UPROPERTY(EditAnywhere, meta = (UMin = 1, ClampMin = 1))
	int numberOfPressedRequire = 1;

	int currentPressedNumber = 0;

	int handle = -1;

public:
	virtual void InitCondition(AMainCharacter* _chara) override;
	virtual void CancelCondition() override;


private:
	UFUNCTION()
	void InputPressed(const FInputActionValue& _value);
};
