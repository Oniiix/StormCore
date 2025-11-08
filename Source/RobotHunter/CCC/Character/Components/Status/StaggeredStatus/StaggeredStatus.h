#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "StaggeredStatus.generated.h"

UCLASS()
class ROBOTHUNTER_API UStaggeredStatus : public UStatus
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> actorToMove;

	float valueMoved;
	FTimerHandle moveTimer;

public:
	UStaggeredStatus();

private:
	void MoveActor();
	
public:
	virtual void ApplyStatus(AActor* _actor) override;
	virtual void UnapplyStatus(AActor* _actor) override;

	virtual void RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor = nullptr) override;

	virtual float GetValue() const override;
};
