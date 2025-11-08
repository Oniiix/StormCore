#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConditionDataAsset.generated.h"

class AMainCharacter;


UCLASS(HideDropdown)
class ROBOTHUNTER_API UConditionDataAsset : public UDataAsset
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValidateCondition, float, _completionPercentage);

protected:
	FOnValidateCondition onValidateCondition;

	TObjectPtr<AMainCharacter> chara = nullptr;

public:
	FORCEINLINE FOnValidateCondition& OnValidateCondition() { return onValidateCondition; }

	UFUNCTION(BlueprintCallable) virtual void InitCondition(AMainCharacter* _chara);

	virtual void CancelCondition();
};
