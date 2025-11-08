#pragma once

#include "CoreMinimal.h"
#include "../../CustomActorComponent.h"
#include "../CustomFSM.h"
#include "FSMComponent.generated.h"


UCLASS()
class ROBOTHUNTER_API UFSMComponent : public UCustomActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "FSM")
	TSubclassOf<UCustomFSM> fsm;

	UPROPERTY()
	TObjectPtr<UCustomFSM> runningFSM;

public:
	FORCEINLINE TObjectPtr<UCustomFSM> GetRunningFSM() const { return runningFSM; }
	UFSMComponent();

private:
	virtual void Update();
	virtual void Stop();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Start();
};
