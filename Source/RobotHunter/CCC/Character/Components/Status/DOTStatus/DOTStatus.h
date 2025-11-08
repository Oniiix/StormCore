#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/StackingStatus.h"
#include "DOTStatus.generated.h"

UCLASS()
class ROBOTHUNTER_API UDOTStatus : public UStackingStatus
{
	GENERATED_BODY()

public:
	virtual void ApplyStatus(AActor* _actor) override;
};
