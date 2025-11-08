#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/StackingStatus.h"
#include "CooldownModifierStatus.generated.h"

#define CD_MODIF_USE_LOG false

class AMembersActor;

UCLASS()
class ROBOTHUNTER_API UCooldownModifierStatus : public UStackingStatus
{
	GENERATED_BODY()

public:
	UCooldownModifierStatus();
	UCooldownModifierStatus(const FStatusParams& _params);

public:
	virtual void RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor = nullptr) override;
	virtual void ApplyStatus(AActor* _actor) override;
};
