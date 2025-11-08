#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "SlowedStatus.generated.h"

class AAI_Base;

UCLASS()
class ROBOTHUNTER_API USlowedStatus : public UStatus
{
	GENERATED_BODY()

	bool hasSpeedModifier;
	float speedModifier;

public:
	USlowedStatus();

private:
	void CheckAndSlowAI(AActor* _actor);
	void SlowAI(AAI_Base* _ai);

public:
	virtual void ApplyStatus(AActor* _actor) override;
	virtual void UnapplyStatus(AActor* _actor) override;


	virtual void RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor = nullptr) override;
};
