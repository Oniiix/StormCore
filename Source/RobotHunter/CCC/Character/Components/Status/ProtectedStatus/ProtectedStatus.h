#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "ProtectedStatus.generated.h"


UCLASS()
class ROBOTHUNTER_API UProtectedStatus : public UStatus
{
	GENERATED_BODY()

public:
	virtual void ApplyStatus(AActor* _actor) override;
	virtual void UnapplyStatus(AActor* _actor) override;
};
