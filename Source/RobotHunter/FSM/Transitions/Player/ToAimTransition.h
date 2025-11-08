#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToAimTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToAimTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition() override;
};
