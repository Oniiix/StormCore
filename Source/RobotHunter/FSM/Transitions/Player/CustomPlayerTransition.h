#pragma once

#include "CoreMinimal.h"
#include "../CustomTransition.h"
#include "CustomPlayerTransition.generated.h"


class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API UCustomPlayerTransition : public UCustomTransition
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AMainCharacter> player;

public:
	UCustomPlayerTransition();

protected:
	void RetrievePlayer();
};