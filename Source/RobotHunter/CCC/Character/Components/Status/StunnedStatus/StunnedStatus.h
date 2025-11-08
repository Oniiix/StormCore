#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "StunnedStatus.generated.h"

UCLASS()
class ROBOTHUNTER_API UStunnedStatus : public UStatus
{
	GENERATED_BODY()

public:
	virtual void ApplyStatus(AActor* _actor) override;

	virtual float GetValue() const override;
};
