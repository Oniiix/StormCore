#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "StackingStatus.generated.h"

UCLASS()
class ROBOTHUNTER_API UStackingStatus : public UStatus
{
	GENERATED_BODY()

protected:
	bool canRemoveStack;
	
public:
	UStackingStatus();
	UStackingStatus(const FStatusParams& _params);

protected:
	void AddStack();
	void RemoveStack();

	virtual void StackBehaviour();
	virtual void MaxStackBehaviour() {}

public:
	virtual void RefreshStatus(const bool _sourceCooldownPhase, AActor* _actor = nullptr) override;
	virtual void StartStatusTimer(const float _duration = -1.0f) override;
};
