#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Transitions/CustomTransition.h"
#include "States/CustomState.h"
#include "CustomFSM.generated.h"

class UFSMComponent;

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UCustomFSM : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCustomState> initialState;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCustomState> currentState;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, UCustomState*> allStates;

	UPROPERTY()
	TObjectPtr<UFSMComponent> owner;

public:
	FORCEINLINE void SetOwner(UFSMComponent* _owner) { owner = _owner; }

	FORCEINLINE TObjectPtr<UCustomState> GetCurrentState() const { return currentState; }
	FORCEINLINE TSubclassOf<UCustomState> GetInitialState() const { return initialState; }
	FORCEINLINE UFSMComponent* GetOwner() const { return owner; }

public:
	UCustomFSM();

private:
	void CreateState(const FString& _key, TSubclassOf<UCustomState> _state);

public:
	virtual void Start();
	virtual void Update();
	virtual void Stop();

	void ChangeState(TSubclassOf<UCustomState> _state);
};
