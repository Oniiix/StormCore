#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToExplorationTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToExplorationTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition() override;
};
