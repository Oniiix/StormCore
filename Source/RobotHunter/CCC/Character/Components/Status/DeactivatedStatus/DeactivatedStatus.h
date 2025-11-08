#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "DeactivatedStatus.generated.h"

#define DEACTIVATED_STATUS_USE_LOG false

class AMembersActor;

UCLASS()
class ROBOTHUNTER_API UDeactivatedStatus : public UStatus
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AMembersActor> deactivatedMember;

public:
	UDeactivatedStatus();
	
public:
	virtual void ApplyStatus(AActor* _actor) override;
	virtual void UnapplyStatus(AActor* _actor) override;
	virtual float GetValue() const override;
};
