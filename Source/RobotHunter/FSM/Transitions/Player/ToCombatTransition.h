#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerTransition.h"
#include "ToCombatTransition.generated.h"

UCLASS()
class ROBOTHUNTER_API UToCombatTransition : public UCustomPlayerTransition
{
	GENERATED_BODY()

public:
	virtual bool IsValidTransition() override;
};
