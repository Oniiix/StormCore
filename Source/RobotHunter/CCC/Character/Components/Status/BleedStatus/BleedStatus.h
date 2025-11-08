#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "BleedStatus.generated.h"

UCLASS()
class ROBOTHUNTER_API UBleedStatus : public UStatus
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> bleedingActor;

	float bleedRate;
	FTimerHandle bleedTimer;

public:
	FORCEINLINE void SetBleedRate(const float _rate) { bleedRate = _rate; }

public:
	UBleedStatus();

private:
	void Bleed();

public:
	virtual void ApplyStatus(AActor* _actor) override;
	virtual void UnapplyStatus(AActor* _actor) override;
};
