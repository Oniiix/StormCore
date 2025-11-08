#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToMenuTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToMenuTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()
	
public:
	virtual bool IsValidTransition() override;
};
