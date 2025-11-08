#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToDownTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToDownTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition() override;
};
