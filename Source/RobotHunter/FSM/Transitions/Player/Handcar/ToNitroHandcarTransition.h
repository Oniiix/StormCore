#pragma once

#include "CoreMinimal.h"
#include "../CustomPlayerTransition.h"
#include "ToNitroHandcarTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToNitroHandcarTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition() override;
};
