

#pragma once

#include "CoreMinimal.h"
#include "../ConditionDataAsset.h"
#include "InputMappingContext.h"
#include "MultipleInputConditionDataAsset.generated.h"

#define MULTIPLE_INPUT_CONDITION_USE_LOG false


UCLASS(BlueprintType)
class ROBOTHUNTER_API UMultipleInputConditionDataAsset : public UConditionDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UInputAction>> inputActions;

	UPROPERTY(EditAnywhere)
	bool pressedSimultaneously = false;

	int currentInputPressed = 0;

	TArray<int> pressedHandles;
	TArray<int> releasedHandles;


public:
	virtual void InitCondition(AMainCharacter* _chara) override;
	virtual void CancelCondition() override;


private:
	void CheckValidation(UEnhancedInputComponent* _inputContext);
};
