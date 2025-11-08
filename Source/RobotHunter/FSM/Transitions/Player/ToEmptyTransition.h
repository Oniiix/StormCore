

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToEmptyTransition.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UToEmptyTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()
	
public:
	virtual bool IsValidTransition() override;
};
