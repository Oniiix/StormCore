

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToBumpTransition.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UToBumpTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()
	

public:
	virtual bool IsValidTransition() override;
};
