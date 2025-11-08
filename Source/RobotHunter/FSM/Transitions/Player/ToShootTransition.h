#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToShootTransition.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UToShootTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()
	
public:
	virtual bool IsValidTransition() override;
};
