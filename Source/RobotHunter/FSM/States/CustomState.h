#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Transitions/CustomTransition.h"
#include "CustomState.generated.h"

class UCustomFSM;

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UCustomState : public UObject
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnter);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExit);
	
	UPROPERTY(EditAnywhere, Category = "Transitions")
	TArray<TSubclassOf<UCustomTransition>> transitions;

	UPROPERTY()
	TObjectPtr<UCustomFSM> fsm;

	UPROPERTY()
	TArray<UCustomTransition*> runningTransitions;

	FOnEnter onEnter;
	FOnUpdate onUpdate;
	FOnExit onExit;

public:
	FORCEINLINE void SetFSM(UCustomFSM* _fsm) { fsm = _fsm; }

	FORCEINLINE FOnEnter& GetOnEnter() { return onEnter; }
	FORCEINLINE FOnUpdate& GetOnUpdate() { return onUpdate; }
	FORCEINLINE FOnExit& GetOnExit() { return onExit; }

public:
	UCustomState();

private:
	void CheckForValidTransition();

public:
	void InitRunningTransitions();

	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};
